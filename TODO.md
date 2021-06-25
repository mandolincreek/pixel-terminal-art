# `pixel`: potential ideas and plans for the future

Drawing modes:
* Freestyle:
  * fix separation of pen/color modes
* Line
* Polygon:
  * Uniform
  * Polyline
* Square/Rectangle
* Oval/Circle
* Curve
  * Oval/circular arcs
  * Beizer
* Spirals
* Diagram/Flowchart Objects:
  * square box with text label
  * diamond box with label
  * (non|uni|bi)-directional arrow with (optional) label
* Text:
  * start with horizontal/vertical/diagonal
  * consider inserting text along an arbitrary path

Color mode:
* support font coloring, not just background
* support custom colors

Pen mode:
* support custom pen shapes, to paint more than a single pixel at a time

Dithering:
* add other algorithms
  * allow overlapping more colors
  * create gradient effects?

Drawing area:
* add command to toggle color palette visibility for more drawing area
* support multiple "pages" of drawings

File Load/Save:
* serialize state to file
* done in a way that paves the way for sending state over the network

Text Export:
* emit sequences of characters of the same color, rather than one at a time

SVG Export:
* export pixel glyph data, not only color

User Interface:
* upgrade to use panels or other existing high-level abstractions?

Network Support:
* peer-to-peer collaborative sessions

Cellular Algorithms:
* evolve drawings based on different algorithms (e.g. Conway's Game of Life)
* pen shapes become templates for different

External Tooling:
* launch a custom terminal for large/custom resolution drawings
