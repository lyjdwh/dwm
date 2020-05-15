rofi -show run -run-list-command $(cat ~/.zsh_aliases |awk -F'[ =]' '{print $2}') -run-command "zsh -i -c '{cmd}'" -rnow
