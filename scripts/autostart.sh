/bin/bash ~/bin/scripts/dwm-status.sh &
/bin/bash ~/bin/scripts/tap-to-click.sh &
/bin/bash ~/bin/scripts/inverse-scroll.sh &
xautolock -time 10 -locker slock &
feh --bg-scale ~/Pictures/steins-gate/1.png
picom -b
nm-applet &
blueman-applet &
xfce4-power-manager &
xfce4-volumed-pulse &
~/bin/scripts/autostart_wait.sh &
