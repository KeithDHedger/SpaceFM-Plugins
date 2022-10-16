#!/bin/bash

#
#©K. D. Hedger. Sun 16 Oct 14:16:50 BST 2022 keithdhedger@gmail.com
#
#This file (exec.sh) is part of Template.
#
#Template is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#Template is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with Template.  If not, see <http://www.gnu.org/licenses/>.
#

$fm_import
CONFIG="$fm_cmd_data"
EXTSCRIPT="${fm_cmd_dir}/extscript"

TEMPLATES=
NAMES=
COUNT=0
THISPROC=$$

while read
	do
		NAMES[$COUNT]=$(basename "${REPLY%%.?*}")
		TEMPLATES[$COUNT]="$REPLY"
		((COUNT++))
	done < <(ls "$HOME/Templates")

RADIOS=""
for ((cnt=0;cnt<COUNT;cnt++))
	do
		RADIOS="${RADIOS} --free-button \"${NAMES[cnt]}\"  set input1 %v -- noop '%(bash -c \" $EXTSCRIPT \\\""${NAMES[cnt]}"\\\" \\\""${TEMPLATES[cnt]}"\\\" $THISPROC\")'"
	done

eval "$(eval "spacefm -g --title \"Choose A Template\" --vbox --compact $RADIOS --input @"/tmp/filename$THISPROC"  --close-box --button ok --button cancel")"

TEMPL="$(cat /tmp/selection$THISPROC)"
SUFFIX="${TEMPL##?*.}"
if [ "$SUFFIX" = "$TEMPL" ];then
	SUFFIX=""
else
	SUFFIX=".$SUFFIX"
fi

cp -r "$HOME/Templates/$(cat "/tmp/selection$THISPROC")" "${dialog_input1}${SUFFIX}"

if [ -e "$HOME/Templates/.extradata" ];then
	while read
		do
			REPLACE=$(echo $REPLY|awk -F= '{print $1}')
			DATA=$(eval echo $REPLY|awk -F= '{print $2}')
			sed -i "s/$REPLACE/$DATA/g" "${dialog_input1}${SUFFIX}"
		done < <(cat "$HOME/Templates/.extradata")
fi

rm /tmp/filename$THISPROC /tmp/selection$THISPROC


