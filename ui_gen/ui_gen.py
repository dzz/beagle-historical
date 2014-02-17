import os

sliders = [ "brush_size", "brush_power", "brush_alpha", "brush_mixpaint", "brush_jitter", "brush_blendmode" ]
make_slider_command = "convert -background transparent -fill black -font Consolas -pointsize 12 label:\" {$slider_name} \" -rotate 90 label_{$slider_name}.bmp & composite -gravity North label_{$slider_name}.bmp be_slider.bmp slider_{$slider_idx}.bmp"

idx = 0;
for slider_name in sliders:
	command = make_slider_command.replace("{$slider_name}",slider_name);
	command = command.replace("{$slider_idx}","%d" % idx);
	os.system(command);
	idx += 1;

buttons = [ "mixpaint", "std","bright","dark" ];
make_button_command = "convert -background transparent -fill black -font Consolas -pointsize 9 label:\"{$button_name}\" label_{$button_name}.bmp & composite -gravity Center label_{$button_name}.bmp button_{$mode}.bmp button_{$button_idx}_{$mode}.bmp"

idx = 0;
for button_name in buttons:
	command = make_button_command.replace("{$button_name}",button_name);
	command = command.replace("{$button_idx}","%d" % idx);

	up_command = command.replace("{$mode}","up");
	down_command = command.replace("{$mode}","down");
	os.system(up_command);
	os.system(down_command);
	idx += 1;
