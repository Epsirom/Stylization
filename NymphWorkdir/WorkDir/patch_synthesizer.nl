require('algorithm.ploader')
require('algorithm.synthesizer')
require('mod.progress')

candidate_image_packages = {
	'farm', 'flower', 'fruit', 'hand', 'sea', 'anime'
}

nymph_loader({
	package = candidate_image_packages[6],
	ld = true,
	seeds = 1024,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {20, 0.0722, 0.7152, 0.2126}
});

patch_radius = math.floor(math.sqrt(style.input.rows * style.input.cols / 10000) / 2)
patch_size = patch_radius * 2 + 1

debug('Patch size: '..patch_size)

init_synthesizer()

offset = {}

pg = progress.create({
	min_value = patch_radius,
	max_value = nymph.input.rows - patch_radius - 1,
	step = 1,
	hint_head = '',
	hint_tail = ''
})

progress.display(pg)

for row = patch_radius, nymph.input.rows - patch_radius - 1 do
	for col = patch_radius, nymph.input.cols - patch_radius - 1 do
		offset[1], offset[2] = get_offset(cor, row, col)
		synthesize_patch_by_luminance({row, col}, offset)
	end
	progress.set_value(pg, row)
end

syncview()
showresult()