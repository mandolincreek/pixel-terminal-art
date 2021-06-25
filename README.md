# pixel: a terminal art project

The `pixel` project provides a simple text-based terminal drawing program.
You can create drawings using either your mouse or the keyboard.  The
grid of on-screen characters serve as pixels in the drawing; these
pixels include both glyph and color information.

Drawings may be saved in multiple formats:
* text: reproduces faithfully using `cat` or `less -r` on modern color terminals
* SVG: produces a rectange for each pixel, with a 2:1 aspect ratio

## Quick Start

* After starting the app, click anywhere to begin.
* Type 'q' to quit anytime.
* Click on the screen to move the mouse cursor (',').
* Click on a color on the lefthand side to select it (or press the number key)
* Switch to drawing mode (see key bindings)
* Click and drag to draw freestyle lines

## Key Bindings

The `pixel` program responds to the following hard-coded key bindings:

|Key|Function|
|----|----|
|`up`, `down`, `left`, `right`|move the cursor around the drawing area|
|`w`|toggles navigation/drawing mode|
|`p`|selects a new character to use for the pen|
|`0`|switches to "pen" drawing model|
|`1` through `8`|switches to "color" drawing mode and selects the color to use|
|'d'|toggles dither mode (0=off, 1=odd, 2=even)|
|'e'|export drawing to `output.txt`|
|'v'|export drawing to `output.svg`|
|`q`|quit the program without saving|

## Dithering

When dithering is `off`, the cursor can paint any pixel.  Otherwise,
this setting determines which cells to mask.  A pixel is masked when
its properties meet the criteria determined by the current dithering mode.

For `odd` and `even` dithering, the sum of the pixel coordinates (`x + y`)
determines the mask, creating two overlapping checkerboard patterns.
Thus, you can blend two colors by drawing in an area using `odd`
dithering for the first color and `even` ditering for the second.
