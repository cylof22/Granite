#ifndef RGTC_H_
#define RGTC_H_

float decode_alpha_rgtc(uvec2 payload, int linear_pixel)
{
	float ep0 = float(int(payload.x & 0xffu)) / 255.0;
	float ep1 = float(((payload.x >> 8) & 0xffu)) / 255.0;
	bool range7 = ep0 > ep1;

	int bit_offset = 16 + linear_pixel * 3;
	uint bits;
	if (bit_offset <= 29)
		bits = bitfieldExtract(payload.x, bit_offset, 3);
	else if (bit_offset >= 32)
		bits = bitfieldExtract(payload.y, bit_offset - 32, 3);
	else
		bits = bitfieldExtract(payload.x, 31, 1) | ((payload.y & 3) << 1);

	float res;

	if (bits < 2)
		res = bits != 0 ? ep1 : ep0;
	else if (range7)
		res = mix(ep0, ep1, (1.0 / 7.0) * float(bits - 1));
	else if (bits > 5)
		res = float(bits & 1);
	else
		res = mix(ep0, ep1, (1.0 / 5.0) * float(bits - 1));

	return res;
}

#endif