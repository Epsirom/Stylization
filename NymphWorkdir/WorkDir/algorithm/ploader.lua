-- Nymph Package Loader

--[[ Use this code fraction to setup loader.

require('algorithm.ploader')

candidate_image_packages = {
	'farm', 'flower', 'fruit', 'hand', 'sea'
}

nymph_loader({
	package = candidate_image_packages[1],
	ld = true,
	seeds = 1024,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {100, 1, 1, 1}
});

]]--

require('algorithm.utils')

function calc_size(obj)
	obj.rows, obj.cols = imgsize(obj.img)
end

function nymph_loader(terms)
	NYMPH_INPUT_PACKAGE = terms['package']
	if terms['ld'] then
		NYMPH_INPUT_PACKAGE = NYMPH_INPUT_PACKAGE..'_ld'
	end
	
	NYMPH_SEEDS = terms['seeds']

	NYMPH_PATCH_ANN_ITERATIONS = terms['patch_match_iterations']
	NYMPH_PATCH_RADIUS = terms['patch_radius']

	NYMPH_DEBUG = terms['debug']

	require('algorithm.config')

	set_energy_pack(terms['energy'], terms[terms['energy']..'_params'])
	debug('Package: '..NYMPH_INPUT_PACKAGE)
	debug('Energy: '..terms['energy'])

	loadimage('style_in', style.input)
	loadimage('style_out', style.output)
	loadimage('nymph_in', nymph.input)
	copymat('nymph_out', 'nymph_in')
	
	style.input = {img = 'style_in', rows = 0, cols = 0}
	calc_size(style.input)
	style.output = {img = 'style_out', rows = 0, cols = 0}
	calc_size(style.output)
	nymph.input = {img = 'nymph_in', rows = 0, cols = 0}
	calc_size(nymph.input)
	nymph.output = {
		img = 'nymph_out', 
		rows = nymph.input.rows, 
		cols = nymph.input.cols
	}

	cor = 'cor'
	load_cor(NYMPH_INPUT_PACKAGE, cor)

	debug('Images loaded.')
end
