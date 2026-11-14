#include "catalog_data.h"

const char* const BRANDS[5] = { "SHEIN EZwear", "SHEIN Curve", "ROMWE", "DAZY", "SHEGLAM" };
const char* const TAGLINES[5] = { "Easy everyday basics for effortless style.", "Fashion with room to be you.",
                                  "Dark, bold and proudly different.", "Trend-forward looks for every day.",
                                  "Glow-up essentials for every shade." };

const Seed SEED[] = {
    { 1001, "Ribbed Knit Crop Tank",      "Tops",        0,  199,  45 },{ 1002, "High-Waist Wide-Leg Jeans",  "Bottoms",     0,  749,  30 },
    { 1003, "Oversized Graphic Tee",      "Tops",        0,  299,  60 },{ 1004, "Drawstring Jogger Pants",    "Bottoms",     0,  449,   8 },
    { 1005, "Casual Zip-Up Hoodie",       "Outerwear",   0,  899,  22 },{ 1006, "Plus Floral Wrap Dress",     "Dresses",     1,  849,  18 },
    { 1007, "Plus Stretch Leggings",      "Bottoms",     1,  349,  40 },{ 1008, "Plus Button-Front Blouse",   "Tops",        1,  499,  25 },
    { 1009, "Plus Denim Jacket",          "Outerwear",   1, 1199,  12 },{ 1010, "Plus Ruffle Hem Maxi Skirt", "Bottoms",     1,  649,   0 },
    { 1011, "Gothic Lace-Up Corset Top",  "Tops",        2,  599,  14 },{ 1012, "Cargo Pants with Chain",     "Bottoms",     2,  999,  20 },
    { 1013, "Plaid Pleated Mini Skirt",   "Bottoms",     2,  449,  33 },{ 1014, "Y2K Graphic Baby Tee",       "Tops",        2,  249,  50 },
    { 1015, "Studded Platform Boots",     "Footwear",    2, 1499,   6 },{ 1016, "Satin Slip Midi Dress",      "Dresses",     3,  799,  16 },
    { 1017, "Cutout Halter Top",          "Tops",        3,  349,  28 },{ 1018, "Lace Trim Cami Set",         "Tops",        3,  399,  35 },
    { 1019, "Pleated Tennis Skirt",       "Bottoms",     3,  449,   3 },{ 1020, "Chunky Chain Shoulder Bag",  "Accessories", 3,  549,  24 },
    { 1021, "Matte Liquid Lipstick",      "Beauty",      4,  249, 120 },{ 1022, "Glow Highlighter Palette",   "Beauty",      4,  399,  55 },
    { 1023, "Volume Mascara",             "Beauty",      4,  199,  80 },{ 1024, "Loose Setting Powder",       "Beauty",      4,  299,  42 },
    { 1025, "Brow Pencil Duo",            "Beauty",      4,  149,   9 } };
const int SEEDS = (int)(sizeof(SEED) / sizeof(SEED[0]));
 