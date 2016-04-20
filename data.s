.export		_music

;_initmusic:	jsr music+$FD

.segment	"MUSIC"
_music:		.incbin "music.dat",2