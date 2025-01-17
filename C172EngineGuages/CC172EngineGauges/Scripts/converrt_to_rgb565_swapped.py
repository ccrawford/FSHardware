import sys
from PIL import Image
import os

def convert_to_rgb565_swapped(image_path):
    try:
        img = Image.open(image_path).convert('RGB')
    except Exception as e:
        print(f"Error opening image: {e}")
        sys.exit(1)
    
    # Extract the base filename without path and extension
    base_name = os.path.splitext(os.path.basename(image_path))[0].upper()
    
    width, height = img.size
    pixel_data = img.load()

    output = []
    output.append(f"// Generated RGB565 C header file with byte-swapped values\n")
    output.append(f"#define {base_name}_IMG_WIDTH {width}\n")
    output.append(f"#define {base_name}_IMG_HEIGHT {height}\n")
    output.append(f"static constexpr uint16_t {base_name}_IMG_DATA[{width} * {height}] PROGMEM= {{\n")

    for y in range(height):
        for x in range(width):
            r, g, b = pixel_data[x, y]
            # Convert to RGB565
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            # Swap bytes
            swapped_rgb565 = ((rgb565 & 0xFF) << 8) | ((rgb565 >> 8) & 0xFF)
            output.append(f"0x{swapped_rgb565:04X}, ")
        output.append("\n")

    output.append("};\n")
    return "".join(output)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python convert_to_rgb565_swapped.py <image_file>")
        sys.exit(1)
    
    image_file = sys.argv[1]
    c_header = convert_to_rgb565_swapped(image_file)
    print(c_header)
