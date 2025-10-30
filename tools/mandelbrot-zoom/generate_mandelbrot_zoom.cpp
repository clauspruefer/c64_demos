/*
 * Generate Mandelbrot endless zoom animation for C64
 * Creates a 4000-frame endless zoom into the Mandelbrot set
 * Output: 320x200 pixel PNG images using Colodore palette
 * 
 * Compile: g++ -O3 -std=c++11 -o generate_mandelbrot_zoom generate_mandelbrot_zoom.cpp -lpng
 */

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <png.h>

// Colodore palette - a more accurate C64 color palette
// Based on Colodore palette by Pepto: https://www.colodore.com/
struct RGB {
    unsigned char r, g, b;
};

const RGB COLODORE_PALETTE_RGB[16] = {
    {0x00, 0x00, 0x00},  // 0: Black
    {0xFF, 0xFF, 0xFF},  // 1: White
    {0x81, 0x33, 0x38},  // 2: Red
    {0x75, 0xCE, 0xC8},  // 3: Cyan
    {0x8E, 0x3C, 0x97},  // 4: Purple
    {0x56, 0xAC, 0x4D},  // 5: Green
    {0x2E, 0x2C, 0x9B},  // 6: Blue
    {0xED, 0xF1, 0x71},  // 7: Yellow
    {0x8E, 0x50, 0x29},  // 8: Orange
    {0x55, 0x38, 0x00},  // 9: Brown
    {0xC4, 0x6C, 0x71},  // 10: Light Red
    {0x4A, 0x4A, 0x4A},  // 11: Dark Grey
    {0x7B, 0x7B, 0x7B},  // 12: Medium Grey
    {0xA9, 0xFF, 0x9F},  // 13: Light Green
    {0x70, 0x6D, 0xEB},  // 14: Light Blue
    {0xB2, 0xB2, 0xB2},  // 15: Light Grey
};

// Create a palette for Mandelbrot rendering (16 colors cycling)
// Using a selection that looks good for fractals
const int MANDELBROT_PALETTE[16] = {
    0,   // Black (deep areas)
    6,   // Blue
    14,  // Light Blue
    3,   // Cyan
    5,   // Green
    13,  // Light Green
    7,   // Yellow
    8,   // Orange
    2,   // Red
    10,  // Light Red
    4,   // Purple
    12,  // Medium Grey
    15,  // Light Grey
    1,   // White
    15,  // Light Grey
    12,  // Medium Grey
};

// Animation parameters
const int WIDTH = 320;        // pixels
const int HEIGHT = 200;       // pixels
const int FRAMES = 4000;      // number of animation frames
const int MAX_ITER = 256;     // maximum iterations for Mandelbrot calculation

// Zoom animation parameters
const double ZOOM_FACTOR = 1.02;  // Zoom factor per frame (smaller = slower zoom)
// Interesting point in the Mandelbrot set to zoom into
// This is a classic zoom target near the "seahorse valley"
const double CENTER_X = -0.743643887037151;
const double CENTER_Y = 0.131825904205330;

/**
 * Calculate Mandelbrot set membership
 * Returns number of iterations before escape (or max_iter if it doesn't escape)
 */
int mandelbrot(double c_real, double c_imag, int max_iter) {
    double z_real = 0.0;
    double z_imag = 0.0;
    
    for (int i = 0; i < max_iter; i++) {
        // Check if we've escaped (|z| > 2)
        if (z_real * z_real + z_imag * z_imag > 4.0) {
            return i;
        }
        
        // z = z^2 + c
        double z_real_new = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2.0 * z_real * z_imag + c_imag;
        z_real = z_real_new;
    }
    
    return max_iter;
}

/**
 * Convert iteration count to palette color index
 * Uses smooth coloring for better gradients
 */
int iteration_to_color(int iteration, int max_iter) {
    if (iteration == max_iter) {
        return 0;  // Black for points in the set
    }
    
    // Smooth coloring with logarithmic mapping
    // This creates nice smooth color gradients
    double smooth_iter = iteration + 1 - std::log(std::log(2.0)) / std::log(2.0);
    
    // Map to palette (cycling through colors)
    int palette_index = static_cast<int>(smooth_iter * 2) % 16;
    
    return MANDELBROT_PALETTE[palette_index];
}

/**
 * Save PNG image using libpng
 */
bool save_png(const std::string& filename, unsigned char* image_data, int width, int height) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return false;
    }
    
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        return false;
    }
    
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return false;
    }
    
    png_init_io(png, fp);
    
    // Set image attributes
    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    
    png_write_info(png, info);
    
    // Write image data
    png_bytep* row_pointers = new png_bytep[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = image_data + y * width * 3;
    }
    
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);
    
    delete[] row_pointers;
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    
    return true;
}

/**
 * Generate one frame of the Mandelbrot zoom animation
 */
void generate_frame(int frame_num, unsigned char* image_data) {
    // Calculate zoom level for this frame
    double zoom = std::pow(ZOOM_FACTOR, frame_num);
    
    // Calculate the scale (size of the complex plane window)
    // Start with a view that shows the full Mandelbrot set
    double initial_scale = 3.0;
    double scale = initial_scale / zoom;
    
    // Render Mandelbrot set
    for (int py = 0; py < HEIGHT; py++) {
        for (int px = 0; px < WIDTH; px++) {
            // Map pixel coordinates to complex plane
            // Center the zoom on our target point
            double x_ratio = (px - WIDTH / 2.0) / (WIDTH / 2.0);
            double y_ratio = (py - HEIGHT / 2.0) / (HEIGHT / 2.0);
            
            double c_real = CENTER_X + x_ratio * scale * (static_cast<double>(WIDTH) / HEIGHT);
            double c_imag = CENTER_Y + y_ratio * scale;
            
            // Calculate Mandelbrot iteration
            int iteration = mandelbrot(c_real, c_imag, MAX_ITER);
            
            // Convert to color
            int color_index = iteration_to_color(iteration, MAX_ITER);
            RGB rgb = COLODORE_PALETTE_RGB[color_index];
            
            // Set pixel color (RGB format)
            int pixel_idx = (py * WIDTH + px) * 3;
            image_data[pixel_idx + 0] = rgb.r;
            image_data[pixel_idx + 1] = rgb.g;
            image_data[pixel_idx + 2] = rgb.b;
        }
    }
}

/**
 * Check if directory exists, create if not
 */
bool ensure_directory(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    
    // Try to create directory
    return mkdir(path.c_str(), 0755) == 0;
}

int main() {
    std::cout << "Generating Mandelbrot endless zoom animation..." << std::endl;
    std::cout << "  Resolution: " << WIDTH << "x" << HEIGHT << " pixels" << std::endl;
    std::cout << "  Frames: " << FRAMES << std::endl;
    std::cout << "  Zoom center: (" << CENTER_X << ", " << CENTER_Y << ")" << std::endl;
    std::cout << "  Zoom factor per frame: " << ZOOM_FACTOR << std::endl;
    std::cout << "  Final zoom level: " << std::scientific << std::pow(ZOOM_FACTOR, FRAMES) << "x" << std::endl;
    
    // Create output directory for PNG frames
    std::string frames_dir = "frames";
    if (!ensure_directory(frames_dir)) {
        std::cerr << "Error: Could not create directory: " << frames_dir << std::endl;
        return 1;
    }
    std::cout << "Output directory: " << frames_dir << "/" << std::endl;
    
    // Allocate image buffer
    unsigned char* image_data = new unsigned char[WIDTH * HEIGHT * 3];
    
    // Generate all frames
    std::cout << "Generating frames..." << std::endl;
    for (int frame = 0; frame < FRAMES; frame++) {
        if (frame % 100 == 0) {
            std::cout << "  Frame " << frame << "/" << FRAMES 
                      << " (" << std::fixed << std::setprecision(1) 
                      << (100.0 * frame / FRAMES) << "%)..." << std::endl;
        }
        
        // Generate frame
        generate_frame(frame, image_data);
        
        // Save frame as PNG
        std::ostringstream filename;
        filename << frames_dir << "/frame_" << std::setfill('0') << std::setw(4) << frame << ".png";
        
        if (!save_png(filename.str(), image_data, WIDTH, HEIGHT)) {
            std::cerr << "Error: Failed to save frame " << frame << std::endl;
            delete[] image_data;
            return 1;
        }
    }
    
    delete[] image_data;
    
    std::cout << "Done! Generated " << FRAMES << " frames" << std::endl;
    std::cout << "PNG frames saved to: " << frames_dir << "/" << std::endl;
    std::cout << "\nTo create a video from frames, you can use ffmpeg:" << std::endl;
    std::cout << "  ffmpeg -framerate 25 -i frames/frame_%04d.png -c:v libx264 -pix_fmt yuv420p mandelbrot_zoom.mp4" << std::endl;
    
    return 0;
}
