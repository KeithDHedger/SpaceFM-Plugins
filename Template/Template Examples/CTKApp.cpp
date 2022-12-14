#if 0

#
#©@USER@ @DATE@ @EMAIL@
#
#CTKApp.cpp is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#at your option) any later version.
#
#DevelopmentFiles is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with DevelopmentFiles.  If not, see <http://www.gnu.org/licenses/>.
#

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s --show-error-list=yes --gen-suppressions="
#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

name=$(basename $0 .cpp)

g++ -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey Magick++ ncurses) -lcursestoolkit "$0" -o $name ||exit 1
echo "done compiling ..."

LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND "./$name" "$@"
retval=$?
rm $name
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesGlobals.h>

CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
CTK_cursesListBoxClass	*lb1=new CTK_cursesListBoxClass(mainApp);
CTK_cursesTextBoxClass	*resulttextbox;
CTK_cursesDropClass		*dropdown;
CTK_cursesEditBoxClass	*edbox;

bool					mbarVis=true;

#define FILEMENU 0
#define QUITITEM 5
#define TABWIDTH 4

void menuselctCB(void *inst,void *userdata)
{
	char				*buffer=(char*)alloca(256);
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);

	edbox->CTK_setRunLoop(false);
	sprintf(buffer,"Menu item (%i) '%s' of menu (%i) '%s' selected.",mc->menuItemNumber,mc->menuNames[mc->menuNumber]->menuItem[mc->menuItemNumber]->menuName,mc->menuNumber,mainApp->menuBar->menuNames[mc->menuNumber]->menuName);
	resulttextbox->CTK_updateText(buffer);

	if((mc->menuNumber==FILEMENU) && (mc->menuItemNumber==QUITITEM))
		mainApp->runEventLoop=false;
	fprintf(stderr,"Gadget type=%i\n",mc->CTK_getGadgetType());
}

void mainloopCBIn(void *mainc,void *data)
{
	//fprintf(stderr,"event loop in\n");
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

void mainloopCBOut(void *mainc,void *data)
{
	//fprintf(stderr,"event loop out\n");
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

void buttonselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	long					ud=(long)userdata;

	sprintf(buffer,"Button '%s' clicked.",bc->label);
	resulttextbox->CTK_updateText(buffer);

	if(ud==1)
		lb1->CTK_selectAll();

	if(ud==2)
		{
			dropdown->CTK_clearList();
			dropdown->CTK_addDropItem("Hide Status Bar");
			dropdown->CTK_addDropItem("Show Status Bar");
			dropdown->CTK_addDropItem("item 30000");
			dropdown->CTK_addDropItem("item 50000");
			lb1->CTK_selectNone();
		}

	if(ud==3)
		{
			mbarVis=!mbarVis;
			mainApp->menuBar->CTK_setMenuBarVisible(mbarVis);
			mainApp->CTK_clearScreen();
			mainApp->CTK_updateScreen(mainApp,SCREENUPDATEALL);
		}
	fprintf(stderr,"Gadget type=%i\n",bc->CTK_getGadgetType());
}

void listselctCB(void *inst,void *userdata)
{
	char						*buffer=(char*)alloca(256);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

	sprintf(buffer,"List %i List item '%s' clicked, user data=%p.",(long)userdata,ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
	resulttextbox->CTK_updateText(buffer);
fprintf(stderr,"getMultipleSelect=%i\n",ls->CTK_getMultipleSelect());
	if(ls->CTK_getMultipleSelect()==true)
		{
			std::vector<bool>	sels=ls->CTK_getSelections();
			for(int j=0;j<sels.size();j++)
				{
					if(sels[j]==true)
						fprintf(stderr,"%s:",ls->listItems[j]->label.c_str());
				}
			fprintf(stderr,"\n");
		}
	fprintf(stderr,"Gadget type=%i\n",ls->CTK_getGadgetType());
}

void checkselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);

	cb->CTK_setValue(!cb->CTK_getValue());
	if((long)userdata==1)
		{
			mainApp->menuBar->CTK_setMenuBarEnable(!cb->CTK_getValue());
		//	mainApp->menuBar->CTK_setMenuBarVisible(!cb->CTK_getValue());
			mainApp->menuBar->enableShortcuts=!cb->CTK_getValue();
		//	mainApp->CTK_clearScreen();
		//	mainApp->CTK_updateScreen(mainApp,NULL);
		}
	if((long)userdata==2)
		mainApp->menuBar->menuNames[1]->menuEnabled=!cb->CTK_getValue();

	sprintf(buffer,"CheckBox '%s' clicked ... Value=%i",cb->label,cb->CTK_getValue());
	resulttextbox->CTK_updateText(buffer);
	fprintf(stderr,"Gadget type=%i\n",cb->CTK_getGadgetType());
}

void dropboxCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesDropClass	*db=static_cast<CTK_cursesDropClass*>(inst);
	sprintf(buffer,"Drop box item=%i label=%s",db->selectedItem,db->label.c_str());
	resulttextbox->CTK_updateText(buffer);
	fprintf(stderr,"Gadget type=%i\n",db->CTK_getGadgetType());

	switch(db->selectedItem)
		{
			case 0:
				edbox->CTK_setStatusBarVisible(false);
				mainApp->CTK_clearScreen();
				mainApp->CTK_updateScreen(mainApp,SCREENUPDATEALL);
				break;
			case 1:
				edbox->CTK_setStatusBarVisible(true);
				break;
		}
}

int main(int argc, char **argv)
{
	CTK_cursesButtonClass			*button;
	CTK_cursesTextBoxClass			*textbox;
	CTK_cursesInputClass			*input;
	CTK_cursesListBoxClass			*list;
	CTK_cursesCheckBoxClass			*checkbox;
	CTK_cursesCheckBoxClass			*checkbox1;
	CTK_cursesEditBoxClass			*editbox;
	CTK_cursesSourceEditBoxClass	*srceditbox;
	CTK_cursesLabelClass			*label;
	CTK_cursesChooserClass			*chooser;
	CTK_cursesFBImageClass			*fbimage;

	const char	*menuNames[]={"File","Edit","Tabs","Navigation","Functions","Bookmarks","Tools","Help",NULL};
	const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
	const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cut L_ine"," _Paste",NULL};
	const char	*navMenuNames[]={" Goto _Define"," _Open Include"," _Goto Line"," Open _Manpage"," _Find"," Find _Next",NULL};
	const char	*helpMenuNames[]={" _Help"," A_bout",NULL};
	
	const char	*sampletxt="Press 'ESC' to activate/deactivate menus.\nUse 'LEFT/RIGHT/IP/DOWN' arrow keys to navigate menus.\n\
'HOME/END/PAGEUP/PAGEDOWN' can also be used.\n\
'RETURN' selects a menu item, 'ESC closes menus without selecting.\n\
Shortcut keys can be used ( without the CTRL key when menus are open )\n\
\n\
'UP/DOWN' etc when a text box is highlighted will scroll the text.\n\
\n\
'TAB' Moves between gadgets.\n\
'SHIFT+TAB' Moves between gadgets backwards.\n\
\n\
'RETURN' when a button is highlighted will 'click' that button.\n\
\n\
'RETURN' when an input box is highlighted will show the cursor and start getting input.\n\
'RETURN' will stop, 'LEFT/RIGHT/HOME/END/BACKSPACE/DELETE' are used for editing.\
\n\
'RETURN' when an edit box is highlighted will begin editing.\n\
'ESC' will exit edit mode.\n\
\n\
Pasting is done via middle click of mouse as usual.\
\n\n\
List two is a multiple selection list, use SPACE to toggle.\n\
The check boxes are the same.\n\
Both lists and checkboxes can have the 'select' key set by the caller.\n\
\n\n\
Drop boxes act the same as menus once selcted in the normal way\n\
";

	coloursStruct cs;
	cs.windowBackCol=BACK_WHITE;
	cs.fancyGadgets=true;

	CTK_cursesUtilsClass	cu;
	cu.CTK_splashScreen(mainApp,"Basic example of CTK gadgets.\nThis is the simple non-blocking splash screen.\nShould be used if your app takes a while to start up.\nIt will disappear in 2 seconds");
	//sleep(2);
//	mainApp->CTK_mainEventLoop(-2000);

//custom menu colours
	cs.hiliteBackCol=BACK_BLACK;
	cs.hiliteForeCol=FORE_GREEN;

	cs.menuHiliteBackCol=BACK_BLACK;
	cs.menuHiliteForeCol=FORE_GREEN;

	cs.menuForeCol=FORE_WHITE;
	cs.menuBackCol=BACK_RED;
	cs.disabledForeCol=FORE_BOLD_RED;

	mainApp->CTK_setTabWidth(TABWIDTH);
	mainApp->CTK_setColours(cs);
	mainApp->CTK_addNewMenuBar();
//	mainApp->CTK_setSelectKey(TERMKEY_SYM_SPACE);

	mainApp->menuBar->CTK_addMenuToBar(menuNames[0]);//0
	mainApp->menuBar->CTK_addMenuToBar(menuNames[1]);//1
	mainApp->menuBar->CTK_addMenuToBar(menuNames[4]);//2
	mainApp->menuBar->CTK_addMenuToBar(menuNames[7]);//3

	int cnt=0;
	while(fileMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(0,fileMenuNames[cnt++]);

	cnt=0;
	while(editMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(1,editMenuNames[cnt++]);

	cnt=0;
	while(navMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(2,navMenuNames[cnt++]);

	cnt=0;
	while(helpMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(3,helpMenuNames[cnt++],false);

	char	buffer[2048];
	for(int j=0;j<100;j++)
		{
			sprintf(buffer," Really long menu, menu item %i",j);
			mainApp->menuBar->CTK_addMenuItem(3,buffer,false);
		}
	mainApp->menuBar->CTK_setSelectCB(menuselctCB,NULL);
	for(int j=2;j<10;j++)
		mainApp->menuBar->menuNames[3]->menuItem[j]->menuEnabled=false;

//reset colours to defaults
	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_setColours(cs);

	textbox=mainApp->CTK_addNewTextBox(3,3,80,8,sampletxt);

//	mainApp->CTK_addNewTextBox(3,3,80,8,true,"/tmp/xxx");
//	mainApp->CTK_addNewTextBox(3,3,80,8,true,"/tmp/manfile");
	resulttextbox=mainApp->CTK_addNewTextBox(3,13,80,1,"Results",false);
	resulttextbox->CTK_setSelectable(false);
//just set box type to inbox.
	cs.textBoxType=INBOX;
	edbox=mainApp->CTK_addNewEditBox(mainApp,101,3,mainApp->maxCols-1-101,8,true,"../ChangeLog");
	edbox->CTK_setColours(cs);

	int 		maxlen=strlen("Toggle Menus");
	std::string	labelstr=cu.CTK_padString(std::string("Select All"),maxlen);
	int			maxwid=80;
	int			startx=1;

//	mainApp->CTK_addNewButton(8,16,30,1,"Select All");
//	mainApp->CTK_addNewButton(8,16,30,1,labelstr.c_str());
	button=mainApp->CTK_addNewButton(cu.CTK_getGadgetPosX(startx,maxwid,3,maxlen,0),16,30,1,labelstr.c_str());
//	mainApp->pages[0].buttons[0]->CTK_setSelectCB(buttonselctCB,(void*)1);
	button->CTK_setSelectCB(buttonselctCB,(void*)1);
//	mainApp->CTK_addNewButton(32,16,11,1,"Select None");
	labelstr=cu.CTK_padString(std::string("Select None"),maxlen);
//	mainApp->CTK_addNewButton(32,16,11,1,labelstr.c_str());
	button=mainApp->CTK_addNewButton(cu.CTK_getGadgetPosX(startx,maxwid,3,maxlen,1),16,11,1,labelstr.c_str());
//	mainApp->pages[0].buttons[1]->CTK_setSelectCB(buttonselctCB,(void*)2);
	button->CTK_setSelectCB(buttonselctCB,(void*)2);
//	mainApp->CTK_addNewButton(56,16,11,1,"Toggle Menus");
	labelstr=cu.CTK_padString(std::string("Toggle Menus"),maxlen);
//	mainApp->CTK_addNewButton(56,16,11,1,labelstr.c_str());
	button=mainApp->CTK_addNewButton(cu.CTK_getGadgetPosX(startx,maxwid,3,maxlen,2),16,11,1,labelstr.c_str());
	button->CTK_setSelectDeselects(false);
	button->CTK_setSelectCB(buttonselctCB,(void*)3);

	input=mainApp->CTK_addNewInput(8,19,36,1,"Some input");
	input->CTK_setColours(cs);
	input->redrawAppWindow=false;

	dropdown=mainApp->CTK_addNewDropDownBox(mainApp,56,19,15,1,"Drop Label    ");
	dropdown->CTK_addDropItem("Hide Status Bar");
	dropdown->CTK_addDropItem("Show Status Bar");
	dropdown->CTK_addDropItem("item 3");
	dropdown->CTK_setColours(cs);
	dropdown->CTK_setSelectCB(dropboxCB,(void*)0xdeadbeef);
	dropdown->CTK_setItemEnabled(2,false);
	dropdown->CTK_setSelectDeselects(false);


//auto add list box to main app
	CTK_cursesListBoxClass	*lb;
	lb=mainApp->CTK_addNewListBox(86,3,10,4);
	lb->CTK_addListItem("Item 1");
	lb->CTK_addListItem("Item 2");
	lb->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	lb->CTK_addListItem("Item 4");
	lb->CTK_addListItem("Item 5");
	lb->CTK_addListItem("Item 6");
	lb->CTK_addListItem("Item 7 is a really long line and gets truncated",(void*)7);
	lb->CTK_setSelectCB(listselctCB,(void*)1);

//use some 256 colours
	cs.foreCol=0;
	cs.backCol=7;
//set hihlite colour to eye watering 256 colour!
	cs.hiliteForeCol=88;
	cs.hiliteBackCol=148;
	cs.use256Colours=true;
	lb->CTK_setColours(cs);

//manualy add list box
//	//CTK_cursesListBoxClass	*lb1=new CTK_cursesListBoxClass();
	lb1->CTK_newListBox(127,13,20,9);
	lb1->CTK_addListItem("Item 1");
	lb1->CTK_addListItem("Item 2");
	lb1->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	lb1->CTK_addListItem("Item 4");
	lb1->CTK_addListItem("Item 5");
	lb1->CTK_addListItem("Item 6");
	lb1->CTK_addListItem("Item 7",(void*)17);
	lb1->CTK_addListItem("Item 8",(void*)18);
	lb1->CTK_addListItem("Item 9 is a really long line and gets truncated",(void*)19);
	lb1->CTK_setSelectCB(listselctCB,(void*)2);

	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	cs.use256Colours=false;

	lb1->CTK_setColours(cs);
	lb1->CTK_setMultipleSelect(true);
	lb1->CTK_setSelectDeselects(false);
	lb1->CTK_setSelectKey(TERMKEY_SYM_SPACE);
	mainApp->CTK_addListBox(lb1);


	label=mainApp->CTK_addNewLabel(85,13,40,2,"Default justified non selectable label.\nLine 2 of label.");
	label=mainApp->CTK_addNewLabel(85,17,40,2,"Centred non selectable label.\nLine 2 of label.");
	label->CTK_setJustify(CENTREJUSTIFY);
	label=mainApp->CTK_addNewLabel(85,21,40,2,"Right justified non selectable label.\nLine 2 of label.");
	label->CTK_setJustify(RIGHTJUSTIFY);

//TODO//???
	checkbox=mainApp->CTK_addNewCheckBox(85,9,10,"Menus Off");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)1);
	//checkbox->CTK_setEnterDeselects(false);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(TERMKEY_SYM_SPACE);
	checkbox=mainApp->CTK_addNewCheckBox(85,11,10,"Edit Off");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)2);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(TERMKEY_SYM_SPACE);


//CTK_cursesGadgetClass*  gad=mainApp->CTK_getGadgetNum(mainApp->pageNumber,TEXTGADGET,2);
////CTK_cursesLabelClass*  lab=static_cast<CTK_cursesLabelClass*>(mainApp->CTK_getGadgetNum(0,LABELGADGET,0));
//fprintf(stderr,"gadget %i=%p text='%s'\n",1,gad,static_cast<CTK_cursesTextBoxClass*>(gad)->CTK_getText().c_str());

	mainApp->CTK_setDefaultGadget(textbox);
//	mainApp->CTK_setDefaultGadget(INPUT,0);
	mainApp->menuBar->CTK_setMenuShortCut(0,5,'Q');
	mainApp->eventLoopCBIn=mainloopCBIn;
	mainApp->eventLoopCBOut=mainloopCBOut;


	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.fancyGadgets=true;
	mainApp->CTK_setColours(cs);


	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;

	delete mainApp;
	return 0;
}
