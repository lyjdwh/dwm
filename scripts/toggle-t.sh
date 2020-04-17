result=$(ps ax|grep -v grep| grep trayer)
if [ "$result" == "" ];
then
    eval "trayer --expand false --edge top --align right --width 20 &"
else
    eval "killall trayer"
fi
