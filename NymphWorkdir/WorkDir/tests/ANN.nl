loadimage('style_in', 'img/sea_ld/style_in.jpg')
loadimage('nymph_in', 'img/sea_ld/nymph_in.jpg')
patchANN('nymph_in', 'style_in', 3, 'offset')
