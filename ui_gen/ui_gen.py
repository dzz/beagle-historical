import os

sliders = [ "size", "dab shape", "alpha", "~ nothing ~", "jitter", "noise" ]
make_slider_command = "convert -background black -fill white -font Consolas -pointsize 12 label:\" {$slider_name} \" -rotate 90 label_{$slider_name}.bmp & composite -gravity Center label_{$slider_name}.bmp be_slider.bmp slider_{$slider_idx}.bmp"

idx = 0;
for slider_name in sliders:
	command = make_slider_command.replace("{$slider_name}",slider_name);
	command = command.replace("{$slider_idx}","%d" % idx);
	os.system(command);
	idx += 1;

buttons = [ "grad","erase", "std","bright","dark" ];
make_button_command = "convert -background transparent -fill {$color} -font Consolas -pointsize 10 label:\"{$button_name}\" label_{$button_name}.bmp & composite -gravity Center label_{$button_name}.bmp button_{$mode}.bmp button_{$button_idx}_{$mode}.bmp"

idx = 0;
for button_name in buttons:
	command = make_button_command.replace("{$button_name}",button_name);
	command = command.replace("{$button_idx}","%d" % idx);

	up_command = command.replace("{$mode}","up").replace("{$color}","black");
	down_command = command.replace("{$mode}","down").replace("{$color}","red");
	os.system(up_command);
	os.system(down_command);
	idx += 1;


mappers = [ "colormix", "alpha", "size", "jitter", "noise" ]
make_mapper_command = "convert -background transparent -fill lime -font Consolas -pointsize 11 label:\" {$mapper_name} \" label_{$mapper_name}.bmp & composite -gravity SouthEast label_{$mapper_name}.bmp mapper.bmp mapper_{$slider_idx}.bmp"

idx = 0;
for mapper_name in mappers:
	command = make_mapper_command.replace("{$mapper_name}",mapper_name);
	command = command.replace("{$slider_idx}","%d" % idx);
	os.system(command);
	idx += 1;
