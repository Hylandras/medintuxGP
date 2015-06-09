posYmenu = 22;
 
 bgcolor = '#354785';
 bgcolor2 = '#5a6ba5';
 
 needcenter = true;
 if(!needcenter)
 posXmenu = 0;
 else
 {
 if (document.all)
 posXmenu = (document.body.clientWidth/2)-(400/2);
 else
 posXmenu = (window.innerWidth/2)-(400/2);
 }
 document.write('<style type="text/css">');
 document.write(' .popper { POSITION: absolute; VISIBILITY: hidden; z-index: 3; }')
 document.write(' #topgauche { position: relative; z-index: 10; }')
 document.write(' A.menu:hover { color: #ffffff; text-decoration: none; }')
 document.write(' A.menu { color: #ffffff; text-decoration: none; }')
 document.write('</style>')
 document.write('<DIV class=popper id=topdeck></DIV>');
 
 t_liens = new Array;
 
 t_liens[0] = new Array;
 t_liens[0][0] = '<A HREF="section0/page0.htm" CLASS=menu>Section 0 / Grand A</A>';
 t_liens[0][1] = '<A HREF="section0/page0.htm" CLASS=menu>Section 0 / P\'tit B</A>';
 
 t_liens[1] = new Array;
 t_liens[1][0] = '<A HREF="section1/page0.htm" CLASS=menu>Section 1 / Page C</A>';
 t_liens[1][1] = '<A HREF="section1/page1.htm" CLASS=menu>Section 1 / Page D</A>';
 t_liens[1][2] = '<A HREF="section1/page2.htm" CLASS=menu>Section 1 / Page E</A>';
 
 t_liens[2] = new Array;
 t_liens[2][0] = '<A HREF="section2/page0.htm" CLASS=menu>Section 2 / Page F</A>';
 t_liens[2][1] = '<A HREF="section2/page1.htm" CLASS=menu>Section 2 / Page G</A>';
 t_liens[2][2] = '<A HREF="section2/page2.htm" CLASS=menu>Section 2 / Page H</A>';
 
 t_liens[3] = new Array;
 t_liens[3][0] = '<A HREF="section3/page0.htm" CLASS=menu>Section 3 / Page I</A>';
 t_liens[3][1] = '<A HREF="section3/page1.htm" CLASS=menu>Section 3 / Page J</A>';
 t_liens[3][2] = '<A HREF="section3/page2.htm" CLASS=menu>Section 3 / Page K</A>';
 t_liens[3][3] = '<A HREF="section3/page3.htm" CLASS=menu>Section 3 / Page L</A>';
 t_liens[3][4] = '<A HREF="section3/page4.htm" CLASS=menu>Section 3 / Page M</A>';
 t_liens[3][5] = '<A HREF="section3/page5.htm" CLASS=menu>Section 3 / Page N</A>';
 
 var nava = (document.layers);
 var dom = (document.getElementById);
 var iex = (document.all);
 if (nava)
 {
 skn = document.topdeck
 }
 else if (dom)
 {
 skn = document.getElementById("topdeck").style
 }
 else if (iex)
 {
 skn = topdeck.style
 }
 skn = document.getElementById("topdeck").style
 skn.top = posYmenu + 20;
 
 function pop(msg,pos)
 {
 skn.visibility = "hidden";
 a=true
 skn.left = posXmenu+pos;
 var content ='<table border=0 cellspacing=1 cellpadding=0 bgcolor="#000000" width=150>';
 pass = 0
 while (pass < msg.length)
 {
 content += "<TR><TD BGCOLOR="+bgcolor+" onMouseOver=\"this.style.background='"+bgcolor2+"'\" onMouseOut=\"this.style.background='"+bgcolor+"'\" align='left' valign='middle'> "+msg[pass]+"</TD></TR>";
 pass++;
 }
 content += "</table>";
 if (nava)
 {
 skn.document.write(content);
 skn.document.close();
 skn.visibility = "visible";
 }
 else if (dom)
 {
 document.getElementById("topdeck").innerHTML = content;
 skn.visibility = "visible";
 }
 else if (iex)
 {
 document.all("topdeck").innerHTML = content;
 skn.visibility = "visible";
 }
 }
 function kill()
 {
 skn.visibility = "hidden";
 }
 document.onclick = kill;
 document.write('<DIV ID=topgauche>')
 
 document.write('<table summary="" border=0 cellspacing=1 cellpadding=0 width=400 align="center" bgcolor="#000000">')
 document.write(' <tr height=20>')
 
 document.write(' <td width=100 align="center" valign="middle" bgcolor='+bgcolor+' onMouseOver="this.style.background=\''+bgcolor2+'\';pop(t_liens[0],0)" onMouseOut="this.style.background=\''+bgcolor+'\'">')
 document.write('<a onMouseOver="pop(t_liens[0],0)" href="section0/index.html" CLASS="menu" title="Section 0">Section 0</a><br>')
 document.write(' </td>')
 
 document.write(' <td width=100 align="center" valign="middle" bgcolor='+bgcolor+' onMouseOver="this.style.background=\''+bgcolor2+'\';pop(t_liens[1],100)" onMouseOut="this.style.background=\''+bgcolor+'\'">')
 document.write('<a onMouseOver="pop(t_liens[1],100)" href="section1/index.html" CLASS="menu" title="Section 1">Section 1</a><br>')
 document.write(' </td>')
 
 document.write(' <td width=100 align="center" valign="middle" bgcolor='+bgcolor+' onMouseOver="this.style.background=\''+bgcolor2+'\';pop(t_liens[2],200)" onMouseOut="this.style.background=\''+bgcolor+'\'">')
 document.write('<a onMouseOver="pop(t_liens[2],200)" href="section2/index.html" CLASS="menu" title="Section 2">Section 2</a><br>')
 document.write(' </td>')
 
 document.write(' <td width=100 align="center" valign="middle" bgcolor='+bgcolor+' onMouseOver="this.style.background=\''+bgcolor2+'\';pop(t_liens[3],300)" onMouseOut="this.style.background=\''+bgcolor+'\'">')
 document.write('<a onMouseOver="pop(t_liens[3],300)" href="section3/index.html" CLASS="menu" title="Section 3">Section 3</a><br>')
 document.write(' </td>')
 
 document.write(' </tr>')
 document.write('</TABLE></DIV>')
 document.close()
 
