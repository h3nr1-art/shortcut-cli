#!/usr/bin/env bash 

if [ "$1" = "create" ] || [ "$1" = "remove" ]; then
	shortcut $1 $2

else 
	if [ "$1" = "show" ]; then 
		shortcut $1
	else 
		cd $HOME/.cache/shortcut/$1 || shortcut help
	fi
	if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then 
		shortcut help
	fi
fi
