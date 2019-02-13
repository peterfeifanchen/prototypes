# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions

EDITOR=vim
alias ctagsGen='ctags -R .'

if [[ -z "${A4_CHROOT}" ]]; then
   export GOROOT=$HOME/bin/go
   export GOPATH=$HOME/go
   export PATH=$GOPATH:$GOPATH/bin:$GOROOT/bin:$GOPATH/bin/linux_386:$PATH
else
   export GOROOT=
fi
function set_prompt(){
   #Colors
   #Reset
   colorReset='\[\e[0m\]'       #Text Reset
   # Bold Colors
   bRed='\[\e[1;31m\]'         #Red
   bGreen='\[\e[1;32m\]'       #Green
   bBlue='\[\e[1;34m\]'        #Blue

   PS1=""

   #Chrooted PWD: Strips down the absolute path to the workspace root
   #if in chrooted environment
   len=${#A4_CHROOT}
   current_dir=`pwd`
   chroot_dir="$current_dir"
   if [ ! -z "$A4_CHROOT" ] && [[ $current_dir = $A4_CHROOT* ]]; then
      chroot_dir=${current_dir:$len}
      if [ -z "$chroot_dir" ]; then
         chroot_dir="/"
      fi
   fi
   chrooted_pwd=`echo "$chroot_dir" 2>/dev/null`
   PS1+="$chrooted_pwd"

   #Adding Info about git repo
   source ~/.git-prompt.sh
   git_info=`__git_ps1`
   if [ ! -z "$git_info" ]; then
      PS1+="$bRed$git_info$colorReset"
   fi

   #Adding Namespace Info
   if [ ! -z "$NSPATH" ]; then
      netns="${NSPATH:1}"
      PS1+="$bGreen[$netns]$colorReset"
   fi

   PS1+="$bBlue \h$colorReset"

   end_marker='> '
   if [ ! -z "$A4_CHROOT" ]; then
      end_marker='% '
   fi
   PS1+="$end_marker"
}

if [ -z "ARTOOLS_NOPROMPTMUNGE" ]; then
PROMPT_COMMAND='set_prompt'
fi
