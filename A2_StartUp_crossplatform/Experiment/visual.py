import tkinter as tk

window = tk.Tk()
window.title("Each fuzzy set on its own coordinates")

width = 920
panel_h = 175
top_margin = 40
height = top_margin + 5 * panel_h + 20

scale_x = 58
scale_y = 90
x_min = -6
x_max = 6

canvas = tk.Canvas(window, width=width, height=height, bg="white")
canvas.pack()

# a, b, c, d from the design table
# NL / PL are shoulders in the plot; the engine left/right types
# only need the sloping edge (otherwise a=b or c=d divides by zero).
sets = [
    {
        "label": "NL",
        "kind": "left shoulder   (flat 1 from -6 to -3, down to 0 at -1.5)",
        "abcd": "a = -6,   b = -6,   c = -3,   d = -1.5",
        "outline": "#1f4e79",
        "fill": "#cfe2f3",
        "points": [(-6, 0), (-6, 1), (-3, 1), (-1.5, 0)],
        "corners": [("a=b", -6, 1), ("c", -3, 1), ("d", -1.5, 0)],
    },
    {
        "label": "NS",
        "kind": "triangle, peak at -1.5",
        "abcd": "a = -3,   b = -1.5,   c = -1.5,   d = 0",
        "outline": "#2e75b6",
        "fill": "#d0e2f3",
        "points": [(-3, 0), (-1.5, 1), (0, 0)],
        "corners": [("a", -3, 0), ("b=c", -1.5, 1), ("d", 0, 0)],
    },
    {
        "label": "ZE",
        "kind": "triangle, peak at 0",
        "abcd": "a = -1.5,   b = 0,   c = 0,   d = 1.5",
        "outline": "#548235",
        "fill": "#e2efda",
        "points": [(-1.5, 0), (0, 1), (1.5, 0)],
        "corners": [("a", -1.5, 0), ("b=c", 0, 1), ("d", 1.5, 0)],
    },
    {
        "label": "PS",
        "kind": "triangle, peak at 1.5",
        "abcd": "a = 0,   b = 1.5,   c = 1.5,   d = 3",
        "outline": "#c65911",
        "fill": "#fce4d6",
        "points": [(0, 0), (1.5, 1), (3, 0)],
        "corners": [("a", 0, 0), ("b=c", 1.5, 1), ("d", 3, 0)],
    },
    {
        "label": "PL",
        "kind": "right shoulder   (up from 0 at 1.5, flat 1 from 3 to 6)",
        "abcd": "a = 1.5,   b = 3,   c = 6,   d = 6",
        "outline": "#c00000",
        "fill": "#f4cccc",
        "points": [(1.5, 0), (3, 1), (6, 1), (6, 0)],
        "corners": [("a", 1.5, 0), ("b", 3, 1), ("c=d", 6, 1)],
    },
]


def to_screen(origin_x, origin_y, x, y):
    return (
        origin_x + x * scale_x,
        origin_y - y * scale_y
    )


def screen_coords(origin_x, origin_y, points):
    coords = []
    for x, y in points:
        sx, sy = to_screen(origin_x, origin_y, x, y)
        coords.extend([sx, sy])
    return coords


def draw_panel(row, trap):
    origin_x = width // 2
    origin_y = top_margin + row * panel_h + 125

    # Panel divider
    canvas.create_line(
        10, top_margin + row * panel_h,
        width - 10, top_margin + row * panel_h,
        fill="#dddddd",
    )

    canvas.create_text(
        20, origin_y - scale_y - 18,
        text=trap["label"],
        fill=trap["outline"],
        font=("Arial", 16, "bold"),
        anchor="w",
    )
    canvas.create_text(
        70, origin_y - scale_y - 22,
        text=trap["kind"],
        fill="#444444",
        font=("Arial", 10),
        anchor="w",
    )
    canvas.create_text(
        70, origin_y - scale_y - 6,
        text=trap["abcd"],
        fill="#444444",
        font=("Courier", 11),
        anchor="w",
    )

    canvas.create_polygon(
        screen_coords(origin_x, origin_y, trap["points"]),
        outline=trap["outline"],
        fill=trap["fill"],
        width=2,
    )

    # Axes for this set
    x0, y0 = to_screen(origin_x, origin_y, x_min - 0.4, 0)
    x1, y1 = to_screen(origin_x, origin_y, x_max + 0.4, 0)
    canvas.create_line(x0, y0, x1, y1, arrow=tk.LAST, width=2)
    canvas.create_text(x1 + 12, y1, text="x")

    ux, uy = to_screen(origin_x, origin_y, 0, 0)
    vx, vy = to_screen(origin_x, origin_y, 0, 1.2)
    canvas.create_line(ux, uy, vx, vy, arrow=tk.LAST, width=2)
    canvas.create_text(vx - 14, vy, text="μ")

    for i in range(x_min, x_max + 1):
        sx, sy = to_screen(origin_x, origin_y, i, 0)
        canvas.create_line(sx, sy - 4, sx, sy + 4)
        canvas.create_text(sx, sy + 14, text=str(i), font=("Arial", 9))

    for mu, name in [(0, "0"), (1, "1")]:
        sx, sy = to_screen(origin_x, origin_y, 0, mu)
        canvas.create_line(sx - 4, sy, sx + 4, sy)
        canvas.create_text(sx - 14, sy, text=name, font=("Arial", 9))

    # a, b, c, d next to the actual corners
    for name, x, y in trap["corners"]:
        sx, sy = to_screen(origin_x, origin_y, x, y)
        dy = -16 if y > 0.5 else 28
        canvas.create_text(
            sx, sy + dy,
            text=name,
            fill=trap["outline"],
            font=("Arial", 12, "bold"),
        )


canvas.create_text(
    width // 2,
    18,
    text="Our first set   |   range -6 to 6   |   same a,b,c,d for input X and input Y",
    font=("Arial", 12),
)

for row, trap in enumerate(sets):
    draw_panel(row, trap)

window.mainloop()
