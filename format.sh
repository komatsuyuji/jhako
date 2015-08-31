#!/bin/bash
home=/opt/future/jhako
cd $home

cd $home/frontends
rake i18n:js:export
bundle exec rake assets:precompile RAILS_ENV=production
systemctl restart httpd

#exit

## add locale file
cd $home
localefile=`find frontends/config/locales/ -type f`
for i in $localefile
do
    chmod -x $i
    svn add $i 2> /dev/null
done

## add makefile
cd $home
makefileam=`find src -name "Makefile.am"`
makefilein=`find src -name "Makefile.in"`
for i in $makefileam $makefilein
do
    chmod 644 $i
    svn add $i 2> /dev/null
done

## add .h .c file
cd $home
cfile=`find src -name "*.c"`
hfile=`find src -name "*.h"`
header=`find include -name "*.h"`
for i in $cfile $hfile $header
do
    echo $i
    /bin/cp $i $i.org
    tr -d '\r' < $i.org > $i
    chmod 644 $i
    rm $i.org
    indent -nut -kr $i
    svn add $i 2> /dev/null
    rm $i~
done

## add ruby file
cd $home
rbfile=`find frontends -name "*.rb" | grep -v 'public/ext'`
for i in $rbfile
do
    echo $i
    /bin/cp $i $i.org
    tr -d '\r' < $i.org > $i
    chmod 644 $i
    svn add $i 2> /dev/null
    rm $i.org
done

## add javascript file
cd $home
jsfile=`find frontends/public/app -name "*.js"`
for i in $jsfile
do
    echo $i
    /bin/cp $i $i.org
    tr -d '\r' < $i.org > $i
    js-beautify -s 2 -o $i.org $i > /dev/null
    /bin/mv $i.org $i
    chmod 644 $i
    svn add $i 2> /dev/null
    #rm $i.org
done

## add js coffee
cd $home
for i in frontends/app/assets/javascripts/*
do
    echo $i
    chmod 644 $i
    svn add $i 2> /dev/null
done

## add stylesheets
cd $home
for i in frontends/app/assets/stylesheets/*
do
    echo $i
    chmod 644 $i
    svn add $i 2> /dev/null
done

## add views
cd $home
for i in frontends/app/views/*
do
    echo $i
    svn add $i 2> /dev/null
done
