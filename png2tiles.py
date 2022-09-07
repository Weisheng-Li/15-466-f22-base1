import imageio.v3 as iio
import numpy
import numpy as np
import sys

# constraint set by PPU446
MAX_PALETTE_COLOR = 4
MAX_TILE = 256

def main():
    # fill with null values such that the dimension will
    # match when performing append. Null value will be
    # deleted when export
    tiles = np.zeros((1, 8, 8))
    palettes = np.zeros((1, 4, 4))
    tp_map = np.zeros((1, 2))  # mapping from tile to palette

    png_list = sys.argv[1:]
    for png_file in png_list:
        png_file = "asset/" + png_file
        im = iio.imread(png_file)
        im = np.flip(im, 0)

        cur_tile = np.zeros((8, 8))
        cur_palette = np.zeros((4, 4))

        color_idx = 0
        color_map = {}
        for i in range(im.shape[0]):
            for j in range(im.shape[1]):
                new_color = tuple(im[i, j, :])
                if new_color in color_map:
                    cur_tile[i, j] = color_map[new_color]
                else:
                    color_map[new_color] = color_idx
                    cur_tile[i, j] = color_idx
                    color_idx += 1

        if len(color_map) > MAX_PALETTE_COLOR:
            print(png_file + ": color exceed MAX_PALETTE_COLOR")
            continue

        for color, idx in color_map.items():
            cur_palette[idx] = list(color)

        cur_tile = np.expand_dims(cur_tile, axis=0)
        cur_palette = np.expand_dims(cur_palette, axis=0)

        tiles = np.append(tiles, cur_tile, axis=0)
        palettes = np.append(palettes, cur_palette, axis=0)
        tp_map = np.append(tp_map, np.array([[len(tiles)-2, len(palettes)-2]]), axis=0)

    tiles = tiles[1:].astype(int)
    palettes = palettes[1:].astype(int)
    tp_map = tp_map[1:].astype(int)

    tiles_count = tiles.shape[0]
    palettes_count = palettes.shape[0]
    assert(tiles.shape[0] == tp_map.shape[0])

    # time to export the result!
    tiles = np.array2string(tiles, separator=" ")
    palettes = np.array2string(palettes, separator=" ")
    tp_map = np.array2string(tp_map, separator=" ")
    for bracket in "[]":
        tiles = tiles.replace(bracket, "")
        palettes = palettes.replace(bracket, "")
        tp_map = tp_map.replace(bracket, "")

    # print(tiles)
    # print(palettes)
    # print(tp_map)

    with open('dist/tiles.asset', 'w+') as f_out:
        f_out.write(str(tiles_count) + '\n')
        f_out.write(tiles + '\n')
        f_out.write(tp_map + '\n')
        f_out.write(str(palettes_count) + '\n')
        f_out.write(palettes)


if __name__ == "__main__":
    main()

