#include <iostream>
#include <cstddef>
#include <jpeglib.h>
#include <ImageReader.h>
#include <stdexcept>
#include <cstring>

void readJpgImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width, unsigned int& height);

void writeJpgImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                  unsigned int width, unsigned int height);

void readImage(const std::string& imagePath, const std::string& format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height)
{
    if (format == "jpg")
    {
        // clean pixels vector since must be fill with pixels from image
        pixels.clear();
        readJpgImage(imagePath, pixels, width, height);
    }
    else
    {
        throw std::runtime_error("Unsupported format: " +format);
    }
}

void writeImage(const std::string& imagePath, const std::string& format,
               const std::vector<ushort>& pixels, unsigned int width,
               unsigned int height)
{
    if (format == "jpg")
    {
        writeJpgImage(imagePath, pixels, width, height);
    }
    else
    {
        throw std::runtime_error("Unsupported format: " +format);
    }
}

void readJpgImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width,
                  unsigned int& height)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE * infile;
    JSAMPARRAY pJpegBuffer;
    int row_stride;
    if ((infile = fopen(imagePath.c_str(), "rb")) == NULL)
    {
        throw std::invalid_argument("Could not open file " +imagePath);
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    width = cinfo.output_width;
    height = cinfo.output_height;

    if (JCS_GRAYSCALE != cinfo.jpeg_color_space)
    {
        throw std::invalid_argument("The image " +imagePath+ " is not greyscale.");
    }

    row_stride = width * cinfo.output_components ;
    pJpegBuffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
        for (unsigned int x = 0; x < width; x++)
        {
            pixels.push_back(pJpegBuffer[0][cinfo.output_components*x]);
        }
    }

    fclose(infile);
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}

void writeJpgImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                   unsigned int width, unsigned int height)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    FILE * outfile = 0;

    if ((outfile = fopen(imagePath.c_str(), "wb")) == 0) {
        throw std::invalid_argument("Could not open file " +imagePath+ " for write.");
    }

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width  = width;
    cinfo.image_height = height;
    cinfo.input_components = 1;
    cinfo.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, true);

    unsigned char* image_buffer = (unsigned char*)malloc
            (cinfo.image_width*cinfo.image_height);

    memcpy(image_buffer,pixels.data(),pixels.size());

    jpeg_start_compress(&cinfo, true);
    unsigned int row_stride = cinfo.image_width;

    JSAMPROW row_pointer[1];
    while (cinfo.next_scanline < cinfo.image_height)
    {
      row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
      (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}
