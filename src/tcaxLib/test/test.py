import tcaxLib

pix = tcaxLib.pix()

test_tuple = ((0, 0), (1, 2), (1, 2, 3, 4, 5, 6, 7, 8))
print(pix.pix_points(test_tuple))

rgba = ((5 << 24) | (3 << 16) | (8 << 8) | 233)
blankPix = pix.BlankPix(5, 5, rgba)
print(blankPix)

print(pix.PixResize(blankPix, 6, 6))
print(pix.PixResizeF(blankPix, 8.5, 8.5))