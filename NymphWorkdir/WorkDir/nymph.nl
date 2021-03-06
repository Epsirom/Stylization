require('algorithm.ploader')
require('algorithm.mostmatch')
require('algorithm.synthesizer')
require('mod.progress')
require('mod.recorder')

nymph_loader({
	-- farm=1, flower=2, fruit=3, hand=4, sea=5, 
	-- anime=6, head=7, ship=8, fat=9, cube=10
	package = 5,
	ld = true,
	seeds = 128,
	patch_radius = 2,
	patch_match_iterations = 3,
	debug = true,
	energy = 'rgb',
	rgb_params = {90, 1, 1, 1}
	--rgb_params = {20, 0.0722, 0.7152, 0.2126}
});

patch_radius = math.floor(math.sqrt(style.input.rows * style.input.cols / 10000) / 2)
patch_size = patch_radius * 2 + 1

debug('Patch size: '..patch_size)

initMT()
init_synthesizer()

--[[
debug('Start most match')
mostmatch()
debug('Most match done.')
debug('Start synthesizing')
synthesize()
debug('Synthesizing done.')
]]--


--copymat(style.output.img, style.input.img)
--copymat(nymph.output.img, nymph.input.img)

rc = recorder.create({directory='synthesis/raw'})

recorder.record(rc, nymph.output.img)

min_mt_remain = style.input.cols * style.input.rows / 10
debug('Start segmentation and synthesizing')
max_mt_remain = mt_remain
total_mt_remain = max_mt_remain - min_mt_remain
percentage_step = 1
debug('0%')
percentage = 0
while mt_remain > min_mt_remain do
	--mostmatch(nymph.output.img, style.output.img)
	mostmatch()
	synthesize()
	current_percentage = (max_mt_remain - mt_remain) / total_mt_remain * 100
	while current_percentage > percentage + percentage_step do
		percentage = percentage + percentage_step
		debug(percentage..'%')
	end
	recorder.record(rc, nymph.output.img)
end
debug('Segmentation and synthesizing done')

debug('Synthesizing remain unsegmented regions')
for row = 0, nymph.input.rows - 1 do
	for col = 0, nymph.input.cols - 1 do
		if not inMT(row, col) then
			synthesize_point({row, col})
		end
	end
end
debug('All synthesized.')

recorder.record(rc, nymph.output.img)

DIPPING_SWITCH = true

if DIPPING_SWITCH then

debug('Start dipping')
init_dipping(patch_size * 2, 3)

pg = progress.create({
	min_value = 0,
	max_value = nymph.input.rows - 1,
	step = 1,
	hint_head = '',
	hint_tail = ''
})
progress.display(pg)
for row = 0, nymph.input.rows - 1 do
	for col = 0, nymph.input.cols - 1 do
		dip_point(row, col)
	end
	progress.set_value(pg, row)
end

saveimage('result/sea_s.jpg', nymph.output.img)
copymat('dipping', nymph.output.img)
for row = 0, nymph.output.rows - 1 do
	for col = 0, nymph.output.cols - 1 do
		local pix = nymph.dip.img[row + 1][col + 1]
		local p = nymph.dip.prob[row + 1][col + 1]
		set_mat_3b('dipping', row, col, pix[1] * p, pix[2] * p, pix[3] * p)
	end
end
imshow('dipping', 'dipping')
combine_dipping()
saveimage('result/sea_dipped.jpg', nymph.output.img)
debug('All dipped.')
end

syncview()
showresult()