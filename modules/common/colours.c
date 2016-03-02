#include <math.h>
#include "colours.h"

uint32_t colour_hsv_to_rgb(uint32_t hsv)
{
	// http://qscribble.blogspot.se/2008/06/integer-conversion-from-hsl-to-rgb.html
	int hue = H_888(hsv);
	int sat = S_888(hsv);
	int lum = V_888(hsv);
	int r, g, b;

	int v;

	v = (lum < 128) ? (lum * (256 + sat)) >> 8 : (((lum + sat) << 8) - lum * sat) >> 8;
	if (v <= 0) {
		r = g = b = 0;
	} else {
		int m;
		int sextant;
		int fract, vsf, mid1, mid2;

		m = lum + lum - v;
		hue *= 6;
		sextant = hue >> 8;
		fract = hue - (sextant << 8);
		vsf = v * fract * (v - m) / v >> 8;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant) {
		case 0:
			r = v;
			g = mid1;
			b = m;
			break;
		case 1:
			r = mid2;
			g = v;
			b = m;
			break;
		case 2:
			r = m;
			g = v;
			b = mid1;
			break;
		case 3:
			r = m;
			g = mid2;
			b = v;
			break;
		case 4:
			r = mid1;
			g = m;
			b = v;
			break;
		case 5:
			r = v;
			g = m;
			b = mid2;
			break;
		}
	}

	return COLOUR_888(r, g, b);
}
