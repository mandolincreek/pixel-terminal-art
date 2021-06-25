#include <curses.h>
#include <stdlib.h>
#include <string.h>

/*
q - quit
w - write
p - pen
d - dither
e - export
v - make svg
arrow keys - move around
bug: sometimes, the color gets messed up and turns into hexadecimal
other bug: current prn is not being displayed
*/

struct User {
	int y, x, prev_y, prev_x, color;
	char moveArrow, pen;
};

struct WindowInfo {
	struct Pixel *board;
	int max_y, max_x;
	bool drawing, erasing;
	char pen, color;
};

struct Pixel {
	char letter, color;
};

static struct Pixel *get_pixel(struct WindowInfo *w, int x, int y) {
	return w->board + y * w->max_x + x;
}
static void set_pixel(struct WindowInfo *w, int x, int y, struct Pixel *p) {
	memcpy(get_pixel(w, x, y), p, sizeof(*p));
}

void debug(const char* msg) {
	FILE *file = fopen("log.txt", "a");
	fputs(msg, file);
	fclose(file);
}

#define ASSIGN_COLOR(color) strColor = color; break;

void makeSquare(FILE* file, short x, short y, char color) {
	char* strColor;
	switch (color) {
		case 0: ASSIGN_COLOR("darkgrey");
		case 1: ASSIGN_COLOR("red");
		case 2: ASSIGN_COLOR("green");
		case 3: ASSIGN_COLOR("yellow");
		case 4: ASSIGN_COLOR("blue");
		case 5: ASSIGN_COLOR("purple");
		case 6: ASSIGN_COLOR("cyan");
		case 7: ASSIGN_COLOR("gray");
		case 8: ASSIGN_COLOR("black");
	}

	fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"1\" height=\"2\" stroke=\"%s\"\n"
		"fill=\"%s\" stroke-width=\"0\"/>\n", x, y * 2, strColor, strColor);
}

void displayInfo(struct WindowInfo wInfo) {
	for (int x = 1; x < wInfo.max_x / 10; x++)
		mvprintw(wInfo.max_y - 4, x, "_");
	for (int y = 3; y >= 1; y--)
		mvprintw(wInfo.max_y - y, wInfo.max_x / 10, "|");

	char cursorMsg[5] = "Pen ";
	cursorMsg[4] = wInfo.pen;

	mvprintw(wInfo.max_y - 3, 2, cursorMsg);

	char* msg;
	if (wInfo.erasing)
		msg = "Erasing";
	else if (wInfo.drawing)
		msg = "Drawing";
	else
		msg = "Moving  ";

	mvprintw(wInfo.max_y - 2, 2, msg);

	char colorMsg[16];
	sprintf(colorMsg, ", Color %d", wInfo.color);
	mvprintw(wInfo.max_y - 3, 7, colorMsg);

	int color_ind = 0;
	for (int y = 1; y < wInfo.max_y - 4; y++) {
		attron(COLOR_PAIR(color_ind));
		mvprintw(y, 1, "*****");
		mvprintw(y, 2, "*****");
		attroff(COLOR_PAIR(color_ind));
		if (y % (wInfo.max_y / 9) == 0)
			color_ind++;

	}
}

void displayEraser(int max_x, int max_y) {
	for (int x = max_x / 10 + 1; x < (max_x / 10) * 2; x++)
		mvprintw(max_y - 4, x, "_");

	for (int y = max_y - 3; y < max_y; y++)
		mvprintw(y, (max_x / 10) * 2, "|");

	for (int y = max_y - 3; y < max_y; y++) {
		for (int x = max_x / 10 + 2; x < (max_x / 10) * 2 - 1; x++) {
			mvprintw(y, x, "*");
		}
	}
	mvprintw(max_y - 2, ((max_x / 10) + ((max_x / 10) * 1.6)) / 2, " Eraser ");
}

bool updateEraserStatus(struct User user, int max_x, int max_y, bool erasing) {
	MEVENT mouseClick;
	if (getmouse(&mouseClick) == OK) {
		for (int y = max_y - 3; y < max_y; y++) {
			for (int x = max_x / 10 + 1; x < (max_x / 10) * 2; x++) {
				if (mouseClick.y == y && mouseClick.x == x) {
					return !erasing;
				}
			}
		}
	}
	return erasing;
}

static bool mouseClicked(struct User *user, struct WindowInfo *wInfo)
{
	MEVENT mouseClick;
	if (getmouse(&mouseClick) != OK)
		return false;

	user->y = mouseClick.y;
	user->x = mouseClick.x;

	int block_x = 7, block_y = wInfo->max_y / 9;
	for (int color = 0; color < 9; color++) {
		if ((user->x < block_x) && (user->y < color * block_y)) {
			user->color = color - 1;
			return true;
		}
	}
	return false;
}


// add a "fill" option

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	mousemask(BUTTON1_CLICKED | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);

	use_default_colors();
	start_color();
	for (int i = 0; i < 8; i++)
		init_pair(i, i, i);
	
	int MAX_Y, MAX_X;
	getmaxyx(stdscr, MAX_Y, MAX_X);
	size_t n_squares = MAX_Y * MAX_X;
	struct Pixel *board = malloc(sizeof(*board) * n_squares);
	if (board == NULL)
		return 1;

	memset(board, 0, n_squares * sizeof(*board));

	box(stdscr, '|', '_');

	bool usingApp = true, drawing = false, erasing = false;
	struct User user = {MAX_Y / 10, MAX_X / 10, MAX_Y / 10, MAX_X / 10, 0, ',', '*'};
	struct WindowInfo wInfo = {board, MAX_Y, MAX_X, drawing, erasing, user.pen, 0};

	bool makeNewPen = false;
	int dithering = 0;

	while (usingApp) {
		user.prev_y = user.y;
		user.prev_x = user.x;

		if (makeNewPen) {
			user.pen = getch();
			makeNewPen = false;
		}

		erasing = updateEraserStatus(user, MAX_X, MAX_Y, erasing);

		if (erasing)
			drawing = false;

		int ch = getch();
		if (ch >= '0' && ch <= '9') {
			user.color = ch - '0';
			continue;
		}
		else switch (ch) {
			case 'q':
				usingApp = false; break;
			case 'w':
				drawing = !drawing; break;
			case 'p':
				makeNewPen = true; break;
			case 'd':
				dithering = (dithering + 1) % 3;
				continue;
			case KEY_MOUSE:
				if (mouseClicked(&user, &wInfo))
					drawing = false;
				break;

			case 'e': {
				fclose(fopen("Out/output.txt", "w"));

				FILE* outFile = fopen("Out/output.txt", "a");
				for (int y = 0; y < MAX_Y; y++) {
					for (int x = 0; x < MAX_X; x++) {
						struct Pixel *p = get_pixel(&wInfo, x, y);
					char buf[20];
					sprintf(buf, "\033[48;5;%dm \033[0m", p->color);
					p->letter == 0 ? fputs(" ", outFile) : fputs(buf, outFile);
				    }
				    fputs("\n", outFile);
				}
				fclose(outFile);
				break;
			}

			case 'v': {
				fclose(fopen("Out/output.svg", "w"));
				FILE* svgFile = fopen("Out/output.svg", "a");
				fprintf(svgFile, "<?xml version=\"1.0\" standalone=\"no\"?>\n\
				<svg width=\"%d\" height=\"%d\" version=\"1.1\"\n\
				xmlns=\"http://www.w3.org/2000/svg\">\n\n", MAX_X, MAX_Y * 2);

				for (int y = 0; y < MAX_Y; y++) {
					for (int x = 0; x < MAX_X; x++) {
						struct Pixel *pixel;
						pixel = get_pixel(&wInfo, x, y);
						if (pixel->letter == '\0')
							continue;
						makeSquare(svgFile, x, y, pixel->color);
					}
				}
				fputs("\n</svg>\n", svgFile);
				fclose(svgFile);
				break;
			}

			case KEY_UP:
				user.y--; break;
			case KEY_DOWN:
				user.y++; break;
			case KEY_LEFT:
				user.x--;
				break;
			case KEY_RIGHT:
				user.x++;
				break;
		}

		if (user.y < 0)
			user.y = 5;
		else if (user.y > MAX_Y)
			user.y = MAX_Y;
		else if (user.x < 0)
			user.x = 0;
		else if (user.y > MAX_X)
			user.x = MAX_X - 1;

		if (drawing) {
			struct Pixel p = {user.pen, user.color};
			if (!dithering || ((user.x + user.y + dithering) % 2))
				set_pixel(&wInfo, user.x, user.y, &p);
		}

		else {
			if (erasing) {
				struct Pixel p = {'Z', 0};
				set_pixel(&wInfo, user.x, user.y, &p);
			}

			mvprintw(user.y, user.x, &user.moveArrow);
			mvprintw(user.y, user.x + 1, " ");
			mvprintw(user.prev_y, user.prev_x, " ");
		}

		for (int y = 0; y < MAX_Y; y++) {
			for (int x = 0; x < MAX_X; x++) {
				struct Pixel *p = get_pixel(&wInfo, x, y);
				attron(COLOR_PAIR(p->color));
				const char terminated[2] = {p->letter, '\0'};
				mvprintw(y, x, terminated);
				attroff(COLOR_PAIR(p->color));
			}
		}

		wInfo.drawing = drawing;
		wInfo.erasing = erasing;
		wInfo.pen = user.pen;
		wInfo.color = user.color; // getting messed up

		displayInfo(wInfo);
		displayEraser(MAX_X, MAX_Y);

		refresh();
		flushinp();
		napms(10);
	}
	endwin();
	free(board);
	return 0;
}
