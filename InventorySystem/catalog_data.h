// catalog_data.h - partner brands, taglines and the seed products
#pragma once

extern const char* const BRANDS[5];
extern const char* const TAGLINES[5];

struct Seed { int id; const char* name; const char* cat; int brand; double price; int stock; };  // brand = index into BRANDS
extern const Seed SEED[];
extern const int  SEEDS;
