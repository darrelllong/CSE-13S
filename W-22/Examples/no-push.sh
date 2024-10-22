#!/usr/bin/env bash
cd ~/repos
for d in *
do
    pushd $d
    git pull
    for a in asgn?
    do
        pushd $a
        for b in *
        do
            if [[ -f $b && $b =~ ^(.*)\.o ]]
            then
                git rm $b
                git commit -am "Do not push binaries ($b)"
                git push
            elif [[ -f $b && $b =~ ^(.*)\.aux ]]
            then
                git rm $b
                git commit -am "Do not push aux files ($b)"
                git push
            elif [[ -f $b && $b =~ ^(.*)\.log ]]
            then
                git rm $b
                git commit -am "Do not push log files ($b)"
                git push
            elif [[ -f $b && ! -s $b && $b != "README.md"  && ! $b =~ ^((.)akefile)|(.*)\.(tex|log|c|h|sh) ]]
            then
                git rm $b
                git commit -am "Do not push zero length files ($b)"
                git push
        elif [[ -f $b && -x $b && $b =~ ^(.)akefile ]]
            then
                chmod -x $b
                git commit -am  "Makefiles should not be executable"
                git push
        elif [[ -f $b && -x $b && $b =~ ^(.*)\.(c|h) ]]
            then
                chmod -x $b
                git commit -am  "Source code (c, h) should not be executable"
                git push
        elif [[ -f $b && -x $b && ! $b =~ ^((.)akefile)|(.*)\.(tex|log|c|h|sh) ]]
            then
                echo "Do not push executables" > DO_NOT_PUSH_EXECUTABLES
                git add DO_NOT_PUSH_EXECUTABLES
                git commit -am "Do not push executables ($b)"
                git push
            fi
        done
        popd
    done
    popd
done
