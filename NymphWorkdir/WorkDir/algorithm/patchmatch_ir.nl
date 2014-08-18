require('algorithm.energy')

set_rgb({100, 1, 1, 1})

pkgs = {
	'farm', 'flower', 'fruit', 'hand', 'sea', 'anime', 'head'
}

iterations = 5

for i = 1, #(pkgs) do
	local pkg = pkgs[i]..'_ld'
	loadimage('style_in', 'img/'..pkg..'/style_in.jpg')
	loadimage('nymph_in', 'img/'..pkg..'/nymph_in.jpg')
	rows, cols = imgsize('style_in')

	patch_radius = math.floor(math.sqrt(rows * cols / 10000) / 2)

	cprint('PatchANN '..pkg)
	cor = 'cor_'..pkg
	patchANN('nymph_in', 'style_in', patch_radius, cor, iterations)
	cprint('PatchANN done')
	savemat('ir/'..pkg..'.xml', cor)
	cprint(pkg..' saved.')
end
