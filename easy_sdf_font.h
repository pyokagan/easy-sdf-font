/*
 * easy_sdf_font.h - devel - public domain
 *
 * =====
 * USAGE
 * =====
 *
 * Copy this file into your project. In ONE C file, write:
 *
 *     #define EASY_SDF_FONT_IMPLEMENTATION
 *
 * before the #include of this file.
 * This expands out the actual implementation into that C file.
 *
 * See demo.c for an example of how to use this.
 */
#ifndef _EASY_SDF_FONT_H_
#define _EASY_SDF_FONT_H_

#ifdef ESFONT_STATIC
#define ESFONT_DEF static
#else
#define ESFONT_DEF extern
#endif

#define ESFONT_NUM_GLYPHS 96

ESFONT_DEF int esfont_FindGlyphIndex(int unicode_codepoint);

ESFONT_DEF void esfont_GetGlyphBitmapBox(int glyph, float scale, int padding, int *ix0, int *iy0, int *ix1, int *iy1);

ESFONT_DEF void esfont_GetCodepointBitmapBox(int codepoint, float scale, int padding, int *ix0, int *iy0, int *ix1, int *iy1);

ESFONT_DEF void esfont_GetGlyphSDF(int glyph, float scale, int padding, unsigned char onedge_value, float pixel_dist_scale, float thickness, unsigned char *pixels, int stride);

ESFONT_DEF void esfont_GetCodepointSDF(int codepoint, float scale, int padding, unsigned char onedge_value, float pixel_dist_scale, float thickness, unsigned char *pixels, int stride);

ESFONT_DEF int esfont_GetGlyphAdvanceWidth(int glyph);

ESFONT_DEF int esfont_GetCodepointAdvanceWidth(int codepoint);

/*
 * The following functions are mainly for demo/example use.
 * They are deliberately implemented naively!
 */

struct esfont_glyph_pack {
    int w, h, padding;
    float scale;
};

struct esfont_tri2 {
    float x0, y0, u0, v0;
    float x1, y1, u1, v1;
    float x2, y2, u2, v2;
    float x3, y3, u3, v3;
    float x4, y4, u4, v4;
    float x5, y5, u5, v5;
};

ESFONT_DEF int esfont_PackGlyphs(unsigned char *pixels, int w, int h, float scale, int padding, unsigned char onedge_value, float pixel_dist_scale, float thickness, struct esfont_glyph_pack *info);

ESFONT_DEF int esfont_GetGlyphTri2(const struct esfont_glyph_pack *info, int glyph, float x, float y, float scale, struct esfont_tri2 *tri2);

ESFONT_DEF int esfont_GetCodepointTri2(const struct esfont_glyph_pack *info, int codepoint, float x, float y, float scale, struct esfont_tri2 *tri2);

ESFONT_DEF int esfont_GetTextTri2(const struct esfont_glyph_pack *info, const char *text, float x, float y, float scale, struct esfont_tri2 *tri2, int maxlen);

#ifdef EASY_SDF_FONT_IMPLEMENTATION

#ifndef ESFONT_sqrtf
#include <math.h>
#define ESFONT_sqrtf(x) sqrtf(x)
#endif

#ifndef ESFONT_memset
#include <string.h>
#define ESFONT_memset(s, c, n) memset(s, c, n)
#endif

#ifndef ESFONT_ifloor
#include <math.h>
#define ESFONT_ifloor(x) ((int)floorf(x))
#define ESFONT_iceil(x) ((int)ceilf(x))
#endif

#define ESFONT_BASELINE 11
#define ESFONT_BBOX_MINX 0
#define ESFONT_BBOX_MINY -10
#define ESFONT_BBOX_MAXX 8
#define ESFONT_BBOX_MAXY 3
static const char esfont_adv[] = {
    7, 6, 3, 3, 7, 5, 6, 7, 2, 4, 4, 5, 7, 2, 7, 2, 4, 7, 5, 7, 6, 6, 6,
    6, 6, 7, 6, 3, 3, 4, 7, 4, 7, 8, 7, 7, 7, 7, 6, 6, 7, 6, 7, 6, 6, 5,
    7, 7, 7, 6, 7, 6, 6, 7, 7, 7, 9, 7, 7, 7, 3, 4, 3, 6, 7, 3, 6, 6, 6,
    6, 6, 6, 6, 6, 4, 4, 5, 3, 9, 6, 6, 6, 6, 5, 5, 5, 6, 7, 9, 6, 6, 6,
    5, 3, 5, 8
};
static const unsigned char esfont_bbox[] = {
    17, 107, 0, 187, 18, 43, 1, 35, 2, 107, 2, 76, 2, 91, 2, 107, 1, 3, 1,
    60, 1, 60, 1, 69, 4, 106, 10, 29, 7, 103, 10, 27, 2, 43, 2, 107, 2,
    75, 2, 107, 2, 91, 2, 91, 2, 91, 2, 91, 2, 91, 2, 107, 2, 91, 4, 26,
    4, 28, 5, 59, 6, 104, 5, 59, 18, 108, 3, 106, 2, 107, 2, 107, 2, 107,
    2, 107, 2, 91, 2, 91, 2, 123, 2, 91, 2, 107, 2, 91, 2, 91, 2, 75, 2,
    107, 2, 107, 2, 107, 2, 91, 2, 107, 2, 91, 2, 91, 2, 107, 2, 107, 2,
    107, 2, 139, 2, 107, 2, 107, 2, 107, 1, 44, 2, 43, 1, 44, 1, 67, 12,
    108, 1, 35, 5, 91, 2, 91, 5, 91, 1, 91, 5, 91, 2, 91, 5, 94, 2, 91, 2,
    75, 2, 62, 2, 75, 2, 43, 5, 139, 5, 91, 5, 91, 5, 94, 5, 94, 5, 75, 5,
    75, 3, 75, 5, 91, 5, 107, 5, 139, 5, 91, 5, 94, 5, 91, 1, 76, 17, 28,
    1, 76, 4, 102
};
static const short esfont_coords_i[] = {
    0, 12, 12, 20, 24, 32, 52, 62, 80, 82, 92, 102, 110, 114, 120, 122,
    126, 128, 146, 152, 164, 182, 188, 202, 220, 224, 248, 266, 274, 284,
    288, 292, 296, 316, 340, 346, 366, 374, 386, 394, 400, 416, 422, 428,
    438, 444, 448, 456, 462, 478, 490, 508, 522, 540, 544, 554, 558, 566,
    570, 576, 582, 588, 590, 596, 600, 602, 604, 620, 632, 642, 654, 670,
    678, 696, 706, 716, 728, 734, 740, 758, 768, 784, 798, 810, 816, 834,
    842, 852, 856, 864, 868, 882, 888, 900, 902, 914, 920
};
static const unsigned char esfont_coords[] = {
    17, 97, 97, 107, 107, 27, 17, 27, 17, 107, 27, 97, 34, 40, 18, 24, 26,
    27, 42, 43, 1, 3, 33, 35, 34, 27, 82, 75, 5, 101, 8, 104, 20, 68, 5,
    20, 5, 6, 6, 23, 23, 55, 55, 72, 72, 73, 73, 58, 10, 58, 34, 44, 82,
    11, 3, 19, 4, 20, 73, 89, 74, 90, 104, 75, 75, 27, 27, 9, 9, 69, 69,
    67, 67, 34, 34, 3, 3, 5, 5, 107, 1, 3, 49, 19, 19, 6, 6, 7, 7, 26, 26,
    60, 1, 35, 35, 54, 54, 55, 55, 42, 42, 12, 33, 37, 3, 67, 1, 69, 65,
    5, 7, 103, 52, 58, 10, 11, 26, 27, 27, 13, 7, 103, 10, 11, 26, 27, 34,
    11, 4, 34, 34, 66, 66, 100, 4, 9, 9, 43, 43, 75, 75, 105, 105, 100,
    26, 83, 3, 34, 34, 43, 11, 75, 3, 18, 18, 66, 66, 83, 83, 84, 84, 11,
    11, 107, 2, 66, 66, 83, 83, 85, 85, 70, 6, 70, 70, 87, 87, 90, 90, 75,
    11, 75, 66, 75, 66, 8, 8, 88, 2, 82, 2, 6, 6, 38, 38, 87, 87, 90, 90,
    75, 11, 75, 18, 66, 3, 18, 3, 10, 10, 27, 27, 75, 75, 90, 90, 87, 87,
    70, 70, 6, 2, 82, 82, 27, 18, 82, 3, 18, 3, 5, 5, 104, 104, 106, 106,
    91, 91, 27, 27, 10, 10, 8, 8, 101, 101, 99, 99, 82, 18, 66, 66, 83,
    83, 90, 90, 75, 75, 11, 18, 3, 3, 6, 6, 23, 23, 87, 4, 5, 20, 21, 9,
    10, 25, 26, 4, 5, 20, 21, 9, 10, 25, 26, 26, 12, 53, 8, 8, 59, 6, 102,
    8, 104, 5, 56, 56, 11, 18, 82, 82, 99, 99, 101, 101, 86, 86, 54, 54,
    39, 39, 41, 41, 57, 59, 60, 43, 44, 37, 40, 40, 72, 72, 69, 69, 37,
    72, 104, 104, 101, 101, 67, 67, 19, 19, 5, 5, 8, 8, 42, 42, 106, 50,
    11, 50, 107, 39, 71, 2, 11, 2, 50, 50, 83, 83, 85, 85, 54, 6, 54, 54,
    103, 103, 106, 106, 91, 91, 11, 34, 98, 34, 6, 6, 43, 43, 107, 2, 11,
    2, 34, 34, 100, 100, 106, 106, 43, 43, 11, 2, 11, 2, 82, 6, 86, 11,
    91, 2, 11, 2, 82, 6, 86, 34, 98, 34, 4, 4, 9, 9, 43, 43, 91, 91, 106,
    106, 103, 71, 119, 2, 11, 6, 86, 82, 91, 2, 98, 50, 59, 11, 107, 18,
    82, 82, 90, 90, 75, 75, 27, 27, 10, 2, 11, 82, 6, 6, 91, 2, 11, 11,
    75, 2, 11, 2, 55, 55, 98, 98, 107, 2, 11, 2, 107, 98, 107, 4, 34, 34,
    66, 66, 100, 4, 9, 9, 43, 43, 75, 75, 105, 105, 100, 2, 11, 2, 66, 66,
    83, 83, 85, 85, 70, 70, 6, 4, 34, 34, 66, 66, 100, 4, 9, 9, 43, 43,
    75, 75, 105, 105, 100, 73, 107, 2, 11, 2, 66, 66, 83, 83, 85, 85, 70,
    70, 6, 38, 91, 18, 82, 18, 3, 3, 5, 5, 22, 22, 70, 70, 87, 87, 90, 90,
    75, 75, 11, 2, 98, 50, 59, 2, 9, 9, 43, 43, 75, 75, 105, 105, 98, 2,
    59, 59, 98, 2, 43, 43, 66, 66, 107, 107, 130, 2, 107, 98, 11, 2, 53,
    53, 98, 53, 59, 2, 98, 98, 11, 11, 107, 1, 33, 1, 12, 12, 44, 2, 43,
    1, 33, 33, 44, 44, 12, 3, 33, 33, 67, 12, 108, 1, 35, 23, 87, 23, 8,
    8, 10, 10, 27, 27, 91, 86, 91, 69, 86, 21, 69, 2, 11, 5, 69, 69, 86,
    86, 90, 90, 75, 75, 11, 21, 85, 21, 8, 8, 9, 9, 27, 27, 91, 21, 85,
    21, 8, 8, 9, 9, 27, 27, 91, 91, 81, 37, 53, 53, 86, 86, 88, 88, 24, 6,
    10, 37, 6, 10, 43, 43, 91, 35, 43, 5, 69, 35, 50, 50, 82, 21, 69, 21,
    6, 6, 10, 10, 27, 27, 91, 69, 86, 86, 93, 93, 78, 78, 14, 21, 69, 69,
    86, 86, 91, 21, 6, 2, 11, 5, 37, 37, 43, 11, 75, 35, 34, 19, 18, 21,
    53, 53, 61, 61, 46, 46, 14, 50, 51, 34, 35, 2, 11, 69, 8, 8, 75, 2,
    18, 18, 27, 27, 43, 21, 6, 5, 11, 21, 53, 53, 70, 70, 75, 70, 85, 85,
    117, 117, 134, 134, 139, 21, 69, 69, 86, 86, 91, 21, 6, 5, 11, 21, 69,
    69, 86, 86, 90, 90, 75, 75, 27, 27, 10, 10, 6, 6, 21, 5, 14, 5, 69,
    69, 86, 86, 90, 90, 75, 75, 27, 27, 10, 21, 85, 21, 8, 8, 9, 9, 27,
    27, 91, 94, 85, 5, 11, 6, 21, 21, 69, 21, 69, 6, 21, 6, 7, 7, 24, 24,
    56, 56, 73, 73, 74, 74, 59, 11, 59, 5, 69, 35, 42, 42, 59, 59, 75, 5,
    10, 10, 27, 27, 75, 75, 90, 85, 91, 5, 59, 59, 101, 5, 43, 43, 69, 69,
    107, 107, 133, 5, 91, 85, 11, 5, 10, 10, 27, 27, 75, 75, 90, 85, 93,
    93, 78, 78, 14, 5, 85, 85, 11, 11, 91, 33, 65, 33, 37, 37, 6, 6, 39,
    39, 44, 44, 76, 17, 28, 1, 33, 33, 37, 37, 70, 70, 39, 39, 44, 44, 12,
    5, 36, 36, 70, 70, 101
};
/* total bytes: 1402 */

ESFONT_DEF int
esfont_FindGlyphIndex(int unicode_codepoint)
{
    if (unicode_codepoint >= ' ' && unicode_codepoint <= '~')
        return unicode_codepoint - ' ' + 1;
    return 0;
}

ESFONT_DEF void
esfont_GetGlyphBitmapBox(int glyph, float scale, int padding, int *ix0, int *iy0, int *ix1, int *iy1)
{
    if (esfont_coords_i[glyph] == esfont_coords_i[glyph+1]) {
        if (ix0) *ix0 = 0;
        if (iy0) *iy0 = 0;
        if (ix1) *ix1 = 0;
        if (iy1) *iy1 = 0;
    } else {
        if (ix0) *ix0 = ESFONT_ifloor((esfont_bbox[2*glyph+0] >> 4) * scale) - padding;
        if (iy0) *iy0 = ESFONT_ifloor(((esfont_bbox[2*glyph+0] & 15) - ESFONT_BASELINE) * scale) - padding;
        if (ix1) *ix1 = ESFONT_iceil((esfont_bbox[2*glyph+1] >> 4) * scale) + padding;
        if (iy1) *iy1 = ESFONT_iceil(((esfont_bbox[2*glyph+1] & 15) - ESFONT_BASELINE) * scale) + padding;
    }
}

ESFONT_DEF void
esfont_GetCodepointBitmapBox(int codepoint, float scale, int padding, int *ix0, int *iy0, int *ix1, int *iy1)
{
    esfont_GetGlyphBitmapBox(esfont_FindGlyphIndex(codepoint), scale, padding, ix0, iy0, ix1, iy1);
}

ESFONT_DEF void
esfont_GetGlyphSDF(int glyph, float scale, int padding, unsigned char onedge_value, float pixel_dist_scale, float thickness, unsigned char *pixels, int stride)
{
    int ix0, iy0, ix1, iy1, w, h, data_start, data_end, i, x, y;

    esfont_GetGlyphBitmapBox(glyph, scale, padding, &ix0, &iy0, &ix1, &iy1);
    w = ix1 - ix0;
    h = iy1 - iy0;
    data_start = esfont_coords_i[glyph];
    data_end = esfont_coords_i[glyph+1];

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            float px = (float)(x + ix0) + 0.5f;
            float py = (float)(y + iy0) + 0.5f;
            float minDist = 999999.0f;
            float val;

            for (i = data_start; i < data_end; i += 2) {
                float ax = (esfont_coords[i+0] >> 4) * scale;
                float ay = ((esfont_coords[i+0] & 15) - ESFONT_BASELINE) * scale;
                float bx = (esfont_coords[i+1] >> 4) * scale;
                float by = ((esfont_coords[i+1] & 15) - ESFONT_BASELINE) * scale;
                float pa_x = px - ax;
                float pa_y = py - ay;
                float ba_x = bx - ax;
                float ba_y = by - ay;
                float t = (pa_x*ba_x + pa_y*ba_y) / (ba_x*ba_x + ba_y*ba_y);
                t = t < 0.0f ? 0.0f : t;
                t = 1.0f < t ? 1.0f : t;
                float h_x = pa_x - ba_x * t;
                float h_y = pa_y - ba_y * t;
                float dist = ESFONT_sqrtf(h_x*h_x + h_y*h_y) - thickness;
                minDist = dist < minDist ? dist : minDist;
            }
            minDist = -minDist;
            val = minDist * pixel_dist_scale + onedge_value;
            val = val < 0.0f ? 0.0f : val;
            val = 255.0f < val ? 255.0f : val;
            pixels[y*stride+x] = (unsigned char)val;
        }
    }
}

ESFONT_DEF void
esfont_GetCodepointSDF(int codepoint, float scale, int padding, unsigned char onedge_value, float pixel_dist_scale, float thickness, unsigned char *pixels, int stride)
{
    esfont_GetGlyphSDF(esfont_FindGlyphIndex(codepoint), scale, padding, onedge_value, pixel_dist_scale, thickness, pixels, stride);
}

ESFONT_DEF int
esfont_GetGlyphAdvanceWidth(int glyph)
{
    return esfont_adv[glyph];
}

ESFONT_DEF int
esfont_GetCodepointAdvanceWidth(int codepoint)
{
    return esfont_GetGlyphAdvanceWidth(esfont_FindGlyphIndex(codepoint));
}

ESFONT_DEF int
esfont_PackGlyphs(unsigned char *pixels, int w, int h, float scale, int padding, unsigned char onedge_value, float pixel_dist_scale, float thickness, struct esfont_glyph_pack *info)
{
    int cell_w = ESFONT_iceil(ESFONT_BBOX_MAXX * scale) - ESFONT_ifloor(ESFONT_BBOX_MINX * scale) + padding + padding;
    int cell_h = ESFONT_iceil(ESFONT_BBOX_MAXY * scale) - ESFONT_ifloor(ESFONT_BBOX_MINY * scale) + padding + padding;
    int num_cells_x = w / cell_w;
    int num_cells_y = h / cell_h;
    int glyph;

    if (num_cells_x * num_cells_y < ESFONT_NUM_GLYPHS)
        return 0;

    ESFONT_memset(pixels, 0, w*h);

    for (glyph = 0; glyph < ESFONT_NUM_GLYPHS; glyph++) {
        int cell_y = glyph / num_cells_x;
        int cell_x = glyph % num_cells_x;
        int x0 = cell_x * cell_w;
        int y0 = cell_y * cell_h;

        esfont_GetGlyphSDF(glyph, scale, padding, onedge_value, pixel_dist_scale, thickness, pixels + (y0 * w + x0), w);
    }

    info->w = w;
    info->h = h;
    info->padding = padding;
    info->scale = scale;
    return 1;
}

ESFONT_DEF int
esfont_GetGlyphTri2(const struct esfont_glyph_pack *info, int glyph, float x, float y, float scale, struct esfont_tri2 *tri2)
{
    int w = info->w;
    int padding = info->padding;
    int cell_w = ESFONT_iceil(ESFONT_BBOX_MAXX * info->scale) - ESFONT_ifloor(ESFONT_BBOX_MINX * info->scale) + padding + padding;
    int cell_h = ESFONT_iceil(ESFONT_BBOX_MAXY * info->scale) - ESFONT_ifloor(ESFONT_BBOX_MINY * info->scale) + padding + padding;
    int num_cells_x = w / cell_w;
    int cell_y = glyph / num_cells_x;
    int cell_x = glyph % num_cells_x;
    int x0 = cell_x * cell_w;
    int y0 = cell_y * cell_h;
    int ix0, ix1, iy0, iy1, iw, ih;
    esfont_GetGlyphBitmapBox(glyph, info->scale, info->padding, &ix0, &iy0, &ix1, &iy1);
    iw = ix1 - ix0;
    ih = iy1 - iy0;
    tri2->x0 = x + ix0 * scale;
    tri2->y0 = y + iy0 * scale;
    tri2->u0 = (float)x0 / info->w;
    tri2->v0 = (float)y0 / info->h;
    tri2->x1 = x + ix0 * scale;
    tri2->y1 = y + iy1 * scale;
    tri2->u1 = (float)x0 / info->w;
    tri2->v1 = (float)(y0 + ih) / info->h;
    tri2->x2 = x + ix1 * scale;
    tri2->y2 = y + iy1 * scale;
    tri2->u2 = (float)(x0 + iw) / info->w;
    tri2->v2 = (float)(y0 + ih) / info->h;
    tri2->x3 = tri2->x2;
    tri2->y3 = tri2->y2;
    tri2->u3 = tri2->u2;
    tri2->v3 = tri2->v2;
    tri2->x4 = x + ix1 * scale;
    tri2->y4 = y + iy0 * scale;
    tri2->u4 = (float)(x0 + iw) / info->w;
    tri2->v4 = (float)y0 / info->h;
    tri2->x5 = tri2->x0;
    tri2->y5 = tri2->y0;
    tri2->u5 = tri2->u0;
    tri2->v5 = tri2->v0;
    return esfont_coords_i[glyph] != esfont_coords_i[glyph+1];
}

ESFONT_DEF int
esfont_GetCodepointTri2(const struct esfont_glyph_pack *info, int codepoint, float x, float y, float scale, struct esfont_tri2 *tri2) {
    return esfont_GetGlyphTri2(info, esfont_FindGlyphIndex(codepoint), x, y, scale, tri2);
}

ESFONT_DEF int
esfont_GetTextTri2(const struct esfont_glyph_pack *info, const char *text, float x, float y, float scale, struct esfont_tri2 *tri2, int maxlen)
{
    int i;
    for (i = 0; *text && i < maxlen; text++) {
        if (esfont_GetCodepointTri2(info, *text, x, y, scale, tri2 + i))
            i++;
        x += ((esfont_GetCodepointAdvanceWidth(*text) * info->scale) + info->padding) * scale;
    }
    return i;
}

#endif /* EASY_SDF_FONT_IMPLEMENTATION */
#endif /* _EASY_SDF_FONT_H_ */
