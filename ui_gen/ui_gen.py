#this is a horrific embarassement - but it works.
import os

sliders = [ "size", "dab shape", "alpha", "~ nothing ~", "jitter", "noise" ]
make_slider_command = "convert -background black -fill white -font Consolas -pointsize 12 label:\" {$slider_name} \" -rotate -90 label_{$slider_name}.bmp & composite -gravity Center label_{$slider_name}.bmp templates/be_slider.bmp generated/slider_{$slider_idx}.bmp"

idx = 0;
for slider_name in sliders:
	command = make_slider_command.replace("{$slider_name}",slider_name);
	command = command.replace("{$slider_idx}","%d" % idx);
	os.system(command);
	idx += 1;

buttons = [ "grad","erase", "std","bright","dark" ];
make_button_command = "convert -background transparent -fill {$color} -font Consolas -pointsize 10 label:\"{$button_name}\" label_{$button_name}.bmp & composite -gravity Center label_{$button_name}.bmp templates/button_{$mode}.bmp generated/button_{$button_idx}_{$mode}.bmp"

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
make_mapper_command = "convert -background transparent -fill green -font Consolas -pointsize 11 label:\" {$mapper_name} \" -rotate 45 label_{$mapper_name}.bmp & composite -gravity Center label_{$mapper_name}.bmp templates/mapper.bmp generated/mapper_{$slider_idx}.bmp"

idx = 0;
for mapper_name in mappers:
	command = make_mapper_command.replace("{$mapper_name}",mapper_name);
	command = command.replace("{$slider_idx}","%d" % idx);
	os.system(command);
	idx += 1;

buttons = [ "brush","erase", "smudge","eyedrop" ];
make_button_command = "convert -background transparent -fill {$color} -font Consolas {$boldmode} -pointsize 11 label:\"{$button_name}\" label_{$button_name}.bmp & composite -gravity Center label_{$button_name}.bmp templates/toolbar_{$mode}.bmp generated/toolbar_{$button_idx}_{$mode}.bmp"

idx = 0;
for button_name in buttons:
	command = make_button_command.replace("{$button_name}",button_name);
	command = command.replace("{$button_idx}","%d" % idx);

	up_command = command.replace("{$mode}","up").replace("{$color}","black").replace("{$boldmode}","");
	down_command = command.replace("{$mode}","down").replace("{$color}","white").replace("$boldmode}","-weight bold");
	os.system(up_command);
	os.system(down_command);
	idx += 1;

titles = [ "stylus","brush_controller","mapper","add" ]
make_title_command = "convert -background rgb(72,72,72) -fill white -font Consolas -size 100x16 label:\" {$title_name} \" generated/title_{$title_name}.bmp"

for title_name in titles:
		command = make_title_command.replace("{$title_name}",title_name);
		os.system(command);

titles = [ "pressure","time","size","color","alpha","jitter","noise","input","output" ]
make_title_command = "convert -background rgb(64,64,64) -fill rgba(165,164,162) -font Consolas -size 60x16  label:\" {$title_name} \" generated/title_{$title_name}.bmp"

for title_name in titles:
		command = make_title_command.replace("{$title_name}",title_name);
		os.system(command);

os.system("copy templates\\*.bmp generated\\");
os.system("del *.bmp");
