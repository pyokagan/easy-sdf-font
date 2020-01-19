/*
 * easy_sdf_font.js - devel - public domain
 */
(function (root, factory) {
    if (typeof define === 'function' && define.amd) {
        define([], factory);
    } else if (typeof module === 'object' && module.exports) {
        module.exports = factory();
    } else {
        root.esfont = factory();
    }
}(typeof self !== 'undefined' ? self : this, function () {
'use strict';

var ESFONT_NUM_GLYPHS = 96;
var ESFONT_BASELINE = 11;
var ESFONT_BBOX_MINX = 0;
var ESFONT_BBOX_MINY = -10;
var ESFONT_BBOX_MAXX = 8;
var ESFONT_BBOX_MAXY = 3;
var esfont_adv = [
    7, 6, 3, 3, 7, 5, 6, 7, 2, 4, 4, 5, 7, 2, 7, 2, 4, 7, 5, 7, 6, 6, 6,
    6, 6, 7, 6, 3, 3, 4, 7, 4, 7, 8, 7, 7, 7, 7, 6, 6, 7, 6, 7, 6, 6, 5,
    7, 7, 7, 6, 7, 6, 6, 7, 7, 7, 9, 7, 7, 7, 3, 4, 3, 6, 7, 3, 6, 6, 6,
    6, 6, 6, 6, 6, 4, 4, 5, 3, 9, 6, 6, 6, 6, 5, 5, 5, 6, 7, 9, 6, 6, 6,
    5, 3, 5, 8
];
var esfont_bbox = [
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
];
var esfont_coords_i = [
    0, 12, 12, 20, 24, 32, 52, 62, 80, 82, 92, 102, 110, 114, 120, 122,
    126, 128, 146, 152, 164, 182, 188, 202, 220, 224, 248, 266, 274, 284,
    288, 292, 296, 316, 340, 346, 366, 374, 386, 394, 400, 416, 422, 428,
    438, 444, 448, 456, 462, 478, 490, 508, 522, 540, 544, 554, 558, 566,
    570, 576, 582, 588, 590, 596, 600, 602, 604, 620, 632, 642, 654, 670,
    678, 696, 706, 716, 728, 734, 740, 758, 768, 784, 798, 810, 816, 834,
    842, 852, 856, 864, 868, 882, 888, 900, 902, 914, 920
];
var esfont_coords = [
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
];
// total bytes: 1402

function findGlyphIndex(unicode_codepoint) {
    if (unicode_codepoint >= 32 && unicode_codepoint <= 126)
        return unicode_codepoint - 32 + 1;
    return 0;
}

function getGlyphBitmapBox(glyph, scale, padding) {
    if (esfont_coords_i[glyph] === esfont_coords_i[glyph+1])
        return { ix0: 0, iy0: 0, ix1: 0, iy1: 0 };
    else
        return {
            ix0: (Math.floor((esfont_bbox[2*glyph+0] >>> 4) * scale) - padding)|0,
            iy0: (Math.floor(((esfont_bbox[2*glyph+0] & 15) - ESFONT_BASELINE) * scale) - padding)|0,
            ix1: (Math.ceil((esfont_bbox[2*glyph+1] >>> 4) * scale) + padding) | 0,
            iy1: (Math.ceil(((esfont_bbox[2*glyph+1] & 15) - ESFONT_BASELINE) * scale) + padding)|0,
        };
}

function getCodepointBitmapBox(codepoint, scale, padding) {
    return getGlyphBitmapBox(findGlyphIndex(codepoint), scale, padding);
}

function getGlyphSDF(glyph, scale, padding, onedge_value, pixel_dist_scale, thickness, pixels, off, stride) {
    var box, w, h, data_start, data_end, i, x, y;

    box = getGlyphBitmapBox(glyph, scale, padding);
    w = box.ix1 - box.ix0;
    h = box.iy1 - box.iy0;
    data_start = esfont_coords_i[glyph];
    data_end = esfont_coords_i[glyph+1];

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            var px = (x + box.ix0) + 0.5;
            var py = (y + box.iy0) + 0.5;
            var minDist = 999999.0;
            var val;

            for (i = data_start; i < data_end; i += 2) {
                var ax = (esfont_coords[i+0] >>> 4) * scale;
                var ay = ((esfont_coords[i+0] & 15) - ESFONT_BASELINE) * scale;
                var bx = (esfont_coords[i+1] >>> 4) * scale;
                var by = ((esfont_coords[i+1] & 15) - ESFONT_BASELINE) * scale;
                var pa_x = px - ax;
                var pa_y = py - ay;
                var ba_x = bx - ax;
                var ba_y = by - ay;
                var t = (pa_x*ba_x + pa_y*ba_y) / (ba_x*ba_x + ba_y*ba_y);
                t = t < 0.0 ? 0.0 : t;
                t = 1.0 < t ? 1.0 : t;
                var h_x = pa_x - ba_x * t;
                var h_y = pa_y - ba_y * t;
                var dist = Math.sqrt(h_x*h_x + h_y*h_y) - thickness;
                minDist = dist < minDist ? dist : minDist;
            }
            minDist = -minDist;
            val = minDist * pixel_dist_scale + onedge_value;
            val = val < 0.0 ? 0.0 : val;
            val = 255.0 < val ? 255.0 : val;
            pixels[off+y*stride+x] = val | 0;
        }
    }
}

function getCodepointSDF(codepoint, scale, padding, onedge_value, pixel_dist_scale, thickness, pixels, off, stride) {
    getGlyphSDF(findGlyphIndex(codepoint), scale, padding, onedge_value, pixel_dist_scale, thickness, pixels, off, stride);
}

function getGlyphAdvanceWidth(glyph) {
    return esfont_adv[glyph];
}

function getCodepointAdvanceWidth(codepoint) {
    return getGlyphAdvanceWidth(findGlyphIndex(codepoint));
}

function packGlyphs(pixels, w, h, scale, padding, onedge_value, pixel_dist_scale, thickness) {
    var cell_w = (Math.ceil(ESFONT_BBOX_MAXX * scale) - Math.floor(ESFONT_BBOX_MINX * scale) + padding + padding)|0;
    var cell_h = (Math.ceil(ESFONT_BBOX_MAXY * scale) - Math.floor(ESFONT_BBOX_MINY * scale) + padding + padding)|0;
    var num_cells_x = (w / cell_w) | 0;
    var num_cells_y = (h / cell_h) | 0;
    var glyph, y, x;

    if (num_cells_x * num_cells_y < ESFONT_NUM_GLYPHS)
        return null;

    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
            pixels[y*w+x] = 0;

    for (glyph = 0; glyph < ESFONT_NUM_GLYPHS; glyph++) {
        var cell_y = (glyph / num_cells_x) | 0;
        var cell_x = (glyph % num_cells_x) | 0;
        var x0 = cell_x * cell_w;
        var y0 = cell_y * cell_h;

        getGlyphSDF(glyph, scale, padding, onedge_value, pixel_dist_scale, thickness, pixels, y0 * w + x0, w);
    }

    return { w: w, h: h, padding: padding, scale: scale };
}

function getGlyphTri2(info, glyph, x, y, scale, tri2, off) {
    var w = info.w;
    var padding = info.padding;
    var cell_w = (Math.ceil(ESFONT_BBOX_MAXX * info.scale) - Math.floor(ESFONT_BBOX_MINX * info.scale) + padding + padding)|0;
    var cell_h = (Math.ceil(ESFONT_BBOX_MAXY * info.scale) - Math.floor(ESFONT_BBOX_MINY * info.scale) + padding + padding)|0;
    var num_cells_x = (w / cell_w) | 0;
    var cell_y = (glyph / num_cells_x) | 0;
    var cell_x = (glyph % num_cells_x) | 0;
    var x0 = cell_x * cell_w;
    var y0 = cell_y * cell_h;
    var box, iw, ih;
    box = getGlyphBitmapBox(glyph, info.scale, info.padding);
    iw = box.ix1 - box.ix0;
    ih = box.iy1 - box.iy0;
    tri2[off+0]= x + (box.ix0) * scale;
    tri2[off+1] = y + (box.iy0) * scale;
    tri2[off+2] = x0 / info.w;
    tri2[off+3] = y0 / info.h;
    tri2[off+4] = x + (box.ix0) * scale;
    tri2[off+5] = y + (box.iy1) * scale;
    tri2[off+6] = x0 / info.w;
    tri2[off+7] = (y0 + ih) / info.h;
    tri2[off+8] = x + (box.ix1) * scale;
    tri2[off+9] = y + (box.iy1) * scale;
    tri2[off+10] = (x0 + iw) / info.w;
    tri2[off+11] = (y0 + ih) / info.h;
    tri2[off+12] = tri2[off+8];
    tri2[off+13] = tri2[off+9];
    tri2[off+14] = tri2[off+10];
    tri2[off+15] = tri2[off+11];
    tri2[off+16] = x + (box.ix1) * scale;
    tri2[off+17] = y + (box.iy0) * scale;
    tri2[off+18] = (x0 + iw) / info.w;
    tri2[off+19] = y0 / info.h;
    tri2[off+20] = tri2[off+0];
    tri2[off+21] = tri2[off+1];
    tri2[off+22] = tri2[off+2];
    tri2[off+23] = tri2[off+3];
    return esfont_coords_i[glyph] !== esfont_coords_i[glyph+1];
}

function getCodepointTri2(info, codepoint, x, y, scale, tri2, off) {
    return getGlyphTri2(info, findGlyphIndex(codepoint), x, y, scale, tri2, off);
}

function getTextTri2(info, text, x, y, scale, tri2, off) {
    var i, j;
    for (i = 0, j = off; i < text.length && j + 24 <= tri2.length; i++) {
        var c = text.codePointAt(i);
        if (getCodepointTri2(info, c, x, y, scale, tri2, j)) {
            j += 24;
        }
        x += ((getCodepointAdvanceWidth(c) * info.scale) + info.padding) * scale;
    }
    return i;
}

return {
    NUM_GLYPHS: ESFONT_NUM_GLYPHS,
    findGlyphIndex: findGlyphIndex,
    getGlyphBitmapBox: getGlyphBitmapBox,
    getCodepointBitmapBox: getCodepointBitmapBox,
    getGlyphSDF: getGlyphSDF,
    getCodepointSDF: getCodepointSDF,
    getGlyphAdvanceWidth: getGlyphAdvanceWidth,
    getCodepointAdvanceWidth: getCodepointAdvanceWidth,
    packGlyphs: packGlyphs,
    getGlyphTri2: getGlyphTri2,
    getCodepointTri2: getCodepointTri2,
    getTextTri2: getTextTri2,
};

})); // end wrapper
