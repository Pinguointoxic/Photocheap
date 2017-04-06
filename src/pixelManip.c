/***************************
*	 PIXEL MANIPULATION	   *
***************************/

	/****************
	 RECUPERER PIXEL
	****************/
void setPixel(BMP* bmp, int i, int j, Pixel p)
{
	assert(bmp && i>=0 && i<bmp->width && j>=0 && j<bmp->height);
	bmp->data[bmp->width*j+i] = p;
}

	/***************
	 ASSIGNER PIXEL
	***************/
Pixel getPixel(BMP* bmp, int i, int j)
{
	assert(bmp && i>=0 && i<bmp->width && j>=0 && j<bmp->height);
	return bmp->data[bmp->width*j+i];
}

	/***************
	 	RGB2HSL
	***************/
HSL RGB2HSL(Pixel p)
{
	HSL 	hsl;
	float	R=((float)p.Red / 255.0), G=((float)p.Green / 255.0), B=((float)p.Blue / 255.0);
	float	r, g, b;
	float	max		= max(R, max(G, B));
	float	min		= min(R, min(G, B));
	float	delta 	= max-min;

	hsl.Light = (max+min)/2.0;

	if(delta == 0.0)
		hsl.Hue = hsl.Sat = 0.0;
	else
	{
		if(hsl.Light < 0.5)
			hsl.Sat = delta / (max+min);
		else
			hsl.Sat = delta / (2-delta);

		r = (((max - R) / 6.0 ) + (delta / 2.0)) / delta;
		g = (((max - G) / 6.0 ) + (delta / 2.0)) / delta;
		b = (((max - B) / 6.0 ) + (delta / 2.0)) / delta;

		if(R == max)
			hsl.Hue = b-g;
		else if(G == max)
			hsl.Hue = (1/3.0) + r-b;
		else if(B == max)
			hsl.Hue = (2/3.0) + g-r;

		if(hsl.Hue < 0.0) hsl.Hue +=1;
		if(hsl.Hue > 1.0) hsl.Hue -=1;
	}
	return hsl;
}

	/***************
	 	HSL2RGB
	***************/
Pixel HSL2RGB(HSL hsl)
{
	Pixel p;
	float a, b;

	if(hsl.Sat == 0.0)
	{
		p.Red	= hsl.Light * 255;
		p.Green = hsl.Light * 255;
		p.Blue	= hsl.Light * 255;
	}
	else
	{
		if(hsl.Light < 0.5)
			b = hsl.Light * (1 + hsl.Sat);
		else
			b = (hsl.Light+hsl.Sat) - (hsl.Light*hsl.Sat);

		a = 2 * hsl.Light - b;

		p.Red	= 255 * Hue2RGB(a, b, hsl.Hue+(1/3.0));
		p.Green = 255 * Hue2RGB(a, b, hsl.Hue);
		p.Blue	= 255 * Hue2RGB(a, b, hsl.Hue-(1/3.0));
	}
	return p;
}

float Hue2RGB(float a, float b, float h)
{
	if(h < 0.0) h +=1;
	if(h > 1.0) h -=1;
	if(( 6 * h ) < 1.0 ) return (a + (b - a) * 6 * h);
	if(( 2 * h ) < 1.0 ) return (b);
	if(( 3 * h ) < 2.0 ) return (a + (b - a) * ((2/3.0) - h) * 6);
	return a;
}
