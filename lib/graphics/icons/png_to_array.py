import sys
from PIL import Image

def read_png(filename):
    # Open PNG file
    with Image.open(filename) as img:
        # Get image dimensions and convert to RGBA mode
        width, height = img.size
        print("width: ", width, "height: ", height)
        img = img.convert('RGBA')

        # Read pixel data from image
        data = img.load()
        image = []
        for y in range(height):
            for x in range(width):
                r, g, b, a = data[x, y]
                if a == 0:
                    # Set fully transparent pixels to black
                    r, g, b = 0, 0, 0
                # Convert 8-bit RGB values to 5-bit and 6-bit values
                r = (r >> 3) & 0x1f
                g = (g >> 2) & 0x3f
                b = (b >> 3) & 0x1f
                # Pack RGB values into a 16-bit value
                pixel = (r << 11) | (g << 5) | b
                image.append(pixel)

    # Output array in C++ syntax
    output = 'uint16_t image[] = {\n'
    for i, pixel in enumerate(image):
        output += f'    0x{pixel:04x},'
        if i % width == width - 1:
            output += '\n'
        else:
            output += ' '
    output += '};\n'

    return output

if __name__ == "__main__":
    print("performing conversion of file: ", sys.argv[1])
    print(read_png(sys.argv[1]))