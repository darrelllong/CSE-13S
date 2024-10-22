#!/usr/bin/env bash
cd $HOME/repos
#cd /tmp/repos.dl
for i in *
do
        pushd $i
        git pull
        for k in asgn*
        do
                pushd $k
                for j in a.out hello pig mathlib-test sorting tsp encode decode banhammer
                do
                        if [ -e $j ]
                        then
                                git rm $j
                                echo STOP_PUSHING_EXECUTABLES
                                touch STOP_PUSHING_EXECUTABLES
                                git add STOP_PUSHING_EXECUTABLES
                                git commit -am "do not push executables ($j)"
                                git push
                        fi
                done
                for j in *.o
                do
                        if [ -e $j ]
                        then
                                git rm $j
                                echo STOP_PUSHING_BINARIES
                                touch STOP_PUSHING_BINARIES
                                git add STOP_PUSHING_BINARIES
                                git commit -am "do not push binaries ($j)"
                                git push
                        fi
                done
                popd
        done
        popd
done
