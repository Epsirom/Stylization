function load_cor(pkgname, imgname)
	loadmat('ir/'..pkgname..'.xml', imgname)
end