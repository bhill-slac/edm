//  edm - extensible display manager

//  Copyright (C) 1999 John W. Sinclair

//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#define __updownButton_cc 1

#include "updownButton.h"
#include "app_pkg.h"
#include "act_win.h"

#include "thread.h"

static void udbtoCancelKp (
  Widget w,
  XtPointer client,
  XtPointer call )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbto->keyPadOpen = 0;

}

static void udbtoSetKpDoubleValue (
  Widget w,
  XtPointer client,
  XtPointer call )
{

int stat;
activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbto->keyPadOpen = 0;

  if ( udbto->kpDest == udbto->kpCoarseDest ) {
    udbto->coarse = udbto->kpDouble;
  }
  else if ( udbto->kpDest == udbto->kpFineDest ) {
    udbto->fine = udbto->kpDouble;
  }
  else if ( udbto->kpDest == udbto->kpRateDest ) {
    udbto->rate = udbto->kpDouble;
    udbto->incrementTimerValue = (int) ( 1000.0 * udbto->rate );
    if ( udbto->incrementTimerValue < 50 ) udbto->incrementTimerValue = 50;
  }
  else if ( udbto->kpDest == udbto->kpValueDest ) {
    if ( udbto->destExists ) {
#ifdef __epics__
      stat = ca_put( DBR_DOUBLE, udbto->destPvId, &udbto->kpDouble );
#endif
    }
  }

}

static void menu_cb (
  Widget w,
  XtPointer client,
  XtPointer call )
{

int stat;
activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  if ( w == udbto->pbCoarse ) {

    udbto->kpDest = udbto->kpCoarseDest;
    udbto->kp.create( udbto->actWin->top,
     udbto->x+udbto->actWin->x+udbto->w,
     udbto->y+udbto->actWin->y+10, "", &udbto->kpDouble,
     (void *) client,
     (XtCallbackProc) udbtoSetKpDoubleValue,
     (XtCallbackProc) udbtoCancelKp );
    udbto->keyPadOpen = 1;

  }
  else if ( w == udbto->pbFine ) {

    udbto->kpDest = udbto->kpFineDest;
    udbto->kp.create( udbto->actWin->top,
     udbto->x+udbto->actWin->x+udbto->w,
     udbto->y+udbto->actWin->y+10, "", &udbto->kpDouble,
     (void *) client,
     (XtCallbackProc) udbtoSetKpDoubleValue,
     (XtCallbackProc) udbtoCancelKp );
    udbto->keyPadOpen = 1;

  }
  else if ( w == udbto->pbRate ) {

    udbto->kpDest = udbto->kpRateDest;
    udbto->kp.create( udbto->actWin->top,
     udbto->x+udbto->actWin->x+udbto->w,
     udbto->y+udbto->actWin->y+10, "", &udbto->kpDouble,
     (void *) client,
     (XtCallbackProc) udbtoSetKpDoubleValue,
     (XtCallbackProc) udbtoCancelKp );
    udbto->keyPadOpen = 1;

  }
  else if ( w == udbto->pbValue ) {

    udbto->kpDest = udbto->kpValueDest;
    udbto->kp.create( udbto->actWin->top,
     udbto->x+udbto->actWin->x+udbto->w,
     udbto->y+udbto->actWin->y+10, "", &udbto->kpDouble,
     (void *) client,
     (XtCallbackProc) udbtoSetKpDoubleValue,
     (XtCallbackProc) udbtoCancelKp );
    udbto->keyPadOpen = 1;

  }
  else if ( w == udbto->pbSave ) {

    if ( udbto->savePvConnected ) {
      stat = ca_put( DBR_DOUBLE, udbto->savePvId, &udbto->curControlV );
    }
    else {
      XBell( udbto->actWin->d, 50 );
    }

  }
  else if ( w == udbto->pbRestore ) {

    if ( udbto->savePvConnected ) {
      stat = ca_put( DBR_DOUBLE, udbto->destPvId, &udbto->curSaveV );
    }
    else {
      XBell( udbto->actWin->d, 50 );
    }

  }

}

static void udbtc_edit_update (
  Widget w,
  XtPointer client,
  XtPointer call )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbto->actWin->setChanged();

  udbto->eraseSelectBoxCorners();
  udbto->erase();

  udbto->fgColor.setColorIndex( udbto->bufFgColor, udbto->actWin->ci );

  udbto->bgColor.setColorIndex( udbto->bufBgColor, udbto->actWin->ci );

  udbto->topShadowColor = udbto->bufTopShadowColor;
  udbto->botShadowColor = udbto->bufBotShadowColor;

  udbto->destPvExpString.setRaw( udbto->bufDestPvName );

  udbto->savePvExpString.setRaw( udbto->bufSavePvName );

  udbto->fineExpString.setRaw( udbto->bufFine );

  udbto->coarseExpString.setRaw( udbto->bufCoarse );

  udbto->label.setRaw( udbto->bufLabel );

  strncpy( udbto->fontTag, udbto->fm.currentFontTag(), 63 );
  udbto->actWin->fi->loadFontTag( udbto->fontTag );
  udbto->fs = udbto->actWin->fi->getXFontStruct( udbto->fontTag );

  udbto->_3D = udbto->buf3D;

  udbto->invisible = udbto->bufInvisible;

  udbto->rate = udbto->bufRate;

  udbto->x = udbto->bufX;
  udbto->sboxX = udbto->bufX;

  udbto->y = udbto->bufY;
  udbto->sboxY = udbto->bufY;

  udbto->w = udbto->bufW;
  udbto->sboxW = udbto->bufW;

  udbto->h = udbto->bufH;
  udbto->sboxH = udbto->bufH;

  udbto->updateDimensions();

}

static void udbtc_edit_apply (
  Widget w,
  XtPointer client,
  XtPointer call )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbtc_edit_update ( w, client, call );
  udbto->refresh( udbto );

}

static void udbtc_edit_ok (
  Widget w,
  XtPointer client,
  XtPointer call )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbtc_edit_update ( w, client, call );
  udbto->ef.popdown();
  udbto->operationComplete();

}

static void udbtc_edit_cancel (
  Widget w,
  XtPointer client,
  XtPointer call )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbto->ef.popdown();
  udbto->operationCancel();

}

static void udbtc_edit_cancel_delete (
  Widget w,
  XtPointer client,
  XtPointer call )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;

  udbto->ef.popdown();
  udbto->operationCancel();
  udbto->erase();
  udbto->deleteRequest = 1;
  udbto->drawAll();

}

#ifdef __epics__

static void udbtc_monitor_dest_connect_state (
  struct connection_handler_args arg )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) ca_puser(arg.chid);

  if ( arg.op == CA_OP_CONN_UP ) {

    udbto->needConnectInit = 1;

  }
  else {

    udbto->destPvConnected = 0;
    udbto->active = 0;
    udbto->bgColor.setDisconnected();
    udbto->needDraw = 1;

  }

  udbto->actWin->appCtx->proc->lock();
  udbto->actWin->addDefExeNode( udbto->aglPtr );
  udbto->actWin->appCtx->proc->unlock();

}

static void udbtc_monitor_save_connect_state (
  struct connection_handler_args arg )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) ca_puser(arg.chid);

  if ( arg.op == CA_OP_CONN_UP ) {

    udbto->needSaveConnectInit = 1;
    udbto->actWin->appCtx->proc->lock();
    udbto->actWin->addDefExeNode( udbto->aglPtr );
    udbto->actWin->appCtx->proc->unlock();

  }
  else {

    udbto->savePvConnected = 0;

  }

}

static void udbtc_controlUpdate (
  struct event_handler_args ast_args )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) ast_args.usr;

  udbto->actWin->appCtx->proc->lock();

  udbto->curControlV = *( (double *) ast_args.dbr );

  if ( udbto->savePvConnected ) {
    if ( !udbto->isSaved && ( udbto->curControlV == udbto->curSaveV ) ) {
      udbto->isSaved = 1;
      udbto->needRefresh = 1;
      udbto->actWin->addDefExeNode( udbto->aglPtr );
    }
    else if ( udbto->isSaved && ( udbto->curControlV != udbto->curSaveV ) ) {
      udbto->isSaved = 0;
      udbto->needRefresh = 1;
      udbto->actWin->addDefExeNode( udbto->aglPtr );
    }
  }

  udbto->actWin->appCtx->proc->unlock();

}

static void udbtc_saveUpdate (
  struct event_handler_args ast_args )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) ast_args.usr;

  udbto->actWin->appCtx->proc->lock();

  udbto->curSaveV = *( (double *) ast_args.dbr );

  if ( !udbto->isSaved && ( udbto->curControlV == udbto->curSaveV ) ) {
    udbto->isSaved = 1;
    udbto->needRefresh = 1;
    udbto->actWin->addDefExeNode( udbto->aglPtr );
  }
  else if ( udbto->isSaved && ( udbto->curControlV != udbto->curSaveV ) ) {
    udbto->isSaved = 0;
    udbto->needRefresh = 1;
    udbto->actWin->addDefExeNode( udbto->aglPtr );
  }

  udbto->actWin->appCtx->proc->unlock();

  udbto->savePvConnected = 1;

}

#endif

static void udbtc_decrement (
  XtPointer client,
  XtIntervalId *id )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;
int stat;
double dval;
Window root, child;
int rootX, rootY, winX, winY;
unsigned int mask;

  XQueryPointer( udbto->actWin->d, XtWindow(udbto->actWin->top), &root, &child,
   &rootX, &rootY, &winX, &winY, &mask );

  if ( !( mask & Button1Mask ) ) {
    udbto->incrementTimerActive = 0;
  }

  if ( !udbto->incrementTimerActive ) {
    udbto->incrementTimer = 0;
    return;
  }

  udbto->incrementTimer = XtAppAddTimeOut(
   udbto->actWin->appCtx->appContext(),
   udbto->incrementTimerValue, udbtc_decrement, client );

  udbto->actWin->appCtx->proc->lock();
  dval = udbto->curControlV;
  udbto->actWin->appCtx->proc->unlock();

  dval -= udbto->coarse;

  if ( udbto->destExists ) {
#ifdef __epics__
  stat = ca_put( DBR_DOUBLE, udbto->destPvId, &dval );
#endif
  }

}

static void udbtc_increment (
  XtPointer client,
  XtIntervalId *id )
{

activeUpdownButtonClass *udbto = (activeUpdownButtonClass *) client;
int stat;
double dval;
Window root, child;
int rootX, rootY, winX, winY;
unsigned int mask;

  XQueryPointer( udbto->actWin->d, XtWindow(udbto->actWin->top), &root, &child,
   &rootX, &rootY, &winX, &winY, &mask );

  if ( !( mask & Button3Mask ) ) {
    udbto->incrementTimerActive = 0;
  }

  if ( !udbto->incrementTimerActive ) {
    udbto->incrementTimer = 0;
    return;
  }

  udbto->incrementTimer = XtAppAddTimeOut(
   udbto->actWin->appCtx->appContext(),
   udbto->incrementTimerValue, udbtc_increment, client );

  udbto->actWin->appCtx->proc->lock();
  dval = udbto->curControlV;
  udbto->actWin->appCtx->proc->unlock();

  dval += udbto->coarse;

  if ( udbto->destExists ) {
#ifdef __epics__
  stat = ca_put( DBR_DOUBLE, udbto->destPvId, &dval );
#endif
  }

}

activeUpdownButtonClass::activeUpdownButtonClass ( void ) {

  name = new char[strlen("activeUpdownButtonClass")+1];
  strcpy( name, "activeUpdownButtonClass" );
  buttonPressed = 0;

  _3D = 1;
  invisible = 0;
  rate = 0.1;
  curSaveV = 0.0;

}

// copy constructor
activeUpdownButtonClass::activeUpdownButtonClass
 ( const activeUpdownButtonClass *source ) {

activeGraphicClass *udbto = (activeGraphicClass *) this;

  udbto->clone( (activeGraphicClass *) source );

  name = new char[strlen("activeUpdownButtonClass")+1];
  strcpy( name, "activeUpdownButtonClass" );

  buttonPressed = 0;

  fgCb = source->fgCb;
  bgCb = source->bgCb;
  topShadowCb = source->topShadowCb;
  botShadowCb = source->botShadowCb;

  strncpy( fontTag, source->fontTag, 63 );

  fs = actWin->fi->getXFontStruct( fontTag );

  fgColor.copy( source->fgColor );
  bgColor.copy( source->bgColor );

  topShadowColor = source->topShadowColor;
  botShadowColor = source->botShadowColor;

  destPvExpString.copy( source->destPvExpString );

  savePvExpString.copy( source->savePvExpString );

  fineExpString.copy( source->fineExpString );

  coarseExpString.copy( source->coarseExpString );

  label.copy( source->label );

  _3D = source->_3D;
  invisible = source->invisible;
  rate = source->rate;

  updateDimensions();

}

int activeUpdownButtonClass::createInteractive (
  activeWindowClass *aw_obj,
  int _x,
  int _y,
  int _w,
  int _h ) {

  actWin = (activeWindowClass *) aw_obj;
  x = _x;
  y = _y;
  w = _w;
  h = _h;

  fgColor.setColorIndex( actWin->defaultTextFgColor, actWin->ci );
  bgColor.setColorIndex( actWin->defaultBgColor, actWin->ci );
  topShadowColor = actWin->defaultTopShadowColor;
  botShadowColor = actWin->defaultBotShadowColor;

  strcpy( fontTag, actWin->defaultBtnFontTag );

  actWin->fi->loadFontTag( fontTag );
  fs = actWin->fi->getXFontStruct( fontTag );

  updateDimensions();

  this->draw();

  this->editCreate();

  return 1;

}

int activeUpdownButtonClass::save (
  FILE *f )
{

int index;

  fprintf( f, "%-d %-d %-d\n", UDBTC_MAJOR_VERSION, UDBTC_MINOR_VERSION,
   UDBTC_RELEASE );

  fprintf( f, "%-d\n", x );
  fprintf( f, "%-d\n", y );
  fprintf( f, "%-d\n", w );
  fprintf( f, "%-d\n", h );

  index = fgColor.pixelIndex();
  fprintf( f, "%-d\n", index );

  index = bgColor.pixelIndex();
  fprintf( f, "%-d\n", index );

  index = topShadowColor;
  fprintf( f, "%-d\n", index );

  index = botShadowColor;
  fprintf( f, "%-d\n", index );

  if ( destPvExpString.getRaw() )
    writeStringToFile( f, destPvExpString.getRaw() );
  else
    writeStringToFile( f, "" );

  if ( fineExpString.getRaw() )
    writeStringToFile( f, fineExpString.getRaw() );
  else
    writeStringToFile( f, "" );

  if ( coarseExpString.getRaw() )
    writeStringToFile( f, coarseExpString.getRaw() );
  else
    writeStringToFile( f, "" );

  if ( label.getRaw() )
    writeStringToFile( f, label.getRaw() );
  else
    writeStringToFile( f, "" );

  fprintf( f, "%-d\n", _3D );

  fprintf( f, "%-d\n", invisible );

  fprintf( f, "%-g\n", rate );

  writeStringToFile( f, fontTag );

  // ver 1.1.0
  if ( savePvExpString.getRaw() )
    writeStringToFile( f, savePvExpString.getRaw() );
  else
    writeStringToFile( f, "" );

  return 1;

}

int activeUpdownButtonClass::createFromFile (
  FILE *f,
  char *name,
  activeWindowClass *_actWin )
{

int index;
int major, minor, release;
char oneName[39+1];
float fval;

  this->actWin = _actWin;

  fscanf( f, "%d %d %d\n", &major, &minor, &release ); actWin->incLine();

  fscanf( f, "%d\n", &x ); actWin->incLine();
  fscanf( f, "%d\n", &y ); actWin->incLine();
  fscanf( f, "%d\n", &w ); actWin->incLine();
  fscanf( f, "%d\n", &h ); actWin->incLine();

  this->initSelectBox(); // call after getting x,y,w,h

  fscanf( f, "%d\n", &index ); actWin->incLine();
  fgColor.setColorIndex( index, actWin->ci );

  fscanf( f, "%d\n", &index ); actWin->incLine();
  bgColor.setColorIndex( index, actWin->ci );

  fscanf( f, "%d\n", &index ); actWin->incLine();
  topShadowColor = index;

  fscanf( f, "%d\n", &index ); actWin->incLine();
  botShadowColor = index;

  readStringFromFile( oneName, 39, f ); actWin->incLine();
  destPvExpString.setRaw( oneName );

  readStringFromFile( oneName, 39, f ); actWin->incLine();
  fineExpString.setRaw( oneName );

  readStringFromFile( oneName, 39, f ); actWin->incLine();
  coarseExpString.setRaw( oneName );

  readStringFromFile( oneName, 39, f ); actWin->incLine();
  label.setRaw( oneName );

  fscanf( f, "%d\n", &_3D ); actWin->incLine();

  fscanf( f, "%d\n", &invisible ); actWin->incLine();

  fscanf( f, "%g\n", &fval ); actWin->incLine();
  rate = (double) fval;

  readStringFromFile( fontTag, 63, f ); actWin->incLine();

  if ( ( major > 1 ) || ( ( major == 1 ) && ( minor > 0 ) ) ) {
    readStringFromFile( oneName, 39, f ); actWin->incLine();
    savePvExpString.setRaw( oneName );
  }

  actWin->fi->loadFontTag( fontTag );
  fs = actWin->fi->getXFontStruct( fontTag );

  updateDimensions();

  return 1;

}

int activeUpdownButtonClass::importFromXchFile (
  FILE *f,
  char *name,
  activeWindowClass *_actWin ){

int fgR, fgG, fgB, bgR, bgG, bgB, more, index;
unsigned int pixel;
char *tk, *gotData, *context, buf[255+1];

  fgR = 0xffff;
  fgG = 0xffff;
  fgB = 0xffff;

  bgR = 0xffff;
  bgG = 0xffff;
  bgB = 0xffff;

  this->actWin = _actWin;

  fgColor.setColorIndex( actWin->defaultTextFgColor, actWin->ci );
  bgColor.setColorIndex( actWin->defaultBgColor, actWin->ci );
  topShadowColor = actWin->defaultTopShadowColor;
  botShadowColor = actWin->defaultBotShadowColor;
  strcpy( fontTag, actWin->defaultBtnFontTag );

  label.setRaw( "" );

  // continue until tag is <eod>

  do {

    gotData = getNextDataString( buf, 255, f );
    if ( !gotData ) {
      actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
      return 0;
    }

    context = NULL;

    tk = strtok_r( buf, " \t\n", &context );
    if ( !tk ) {
      actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
      return 0;
    }

    if ( strcmp( tk, "<eod>" ) == 0 ) {

      more = 0;

    }
    else {

      more = 1;

      if ( strcmp( tk, "x" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        x = atol( tk );

      }
      else if ( strcmp( tk, "y" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        y = atol( tk );

      }
      else if ( strcmp( tk, "w" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        w = atol( tk );

      }
      else if ( strcmp( tk, "h" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        h = atol( tk );

      }
            
      else if ( strcmp( tk, "fgred" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        fgR = atol( tk );

      }
            
      else if ( strcmp( tk, "fggreen" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        fgG = atol( tk );

      }
            
      else if ( strcmp( tk, "fgblue" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        fgB = atol( tk );

      }
            
      else if ( strcmp( tk, "bgred" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        bgR = atol( tk );

      }
            
      else if ( strcmp( tk, "bggreen" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        bgG = atol( tk );

      }
            
      else if ( strcmp( tk, "bgblue" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        bgB = atol( tk );

      }
            
      else if ( strcmp( tk, "invisible" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        invisible = atol( tk );

      }
            
      else if ( strcmp( tk, "rate" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        rate = atof( tk );

      }
            
      else if ( strcmp( tk, "font" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n", &context );
        if ( !tk ) {
          actWin->appCtx->postMessage( activeUpdownButtonClass_str1 );
          return 0;
        }

        strncpy( fontTag, tk, 63 );

      }

      else if ( strcmp( tk, "controlpv" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n \t", &context );
        if ( tk ) {
          strncpy( bufDestPvName, tk, 28 );
          bufDestPvName[28] = 0;
          destPvExpString.setRaw( bufDestPvName );
	}

      }

      else if ( strcmp( tk, "fine" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n \t", &context );
        if ( tk ) {
          strncpy( bufFine, tk, 28 );
          bufFine[28] = 0;
          fineExpString.setRaw( bufFine );
	}

      }

      else if ( strcmp( tk, "coarse" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n \t", &context );
        if ( tk ) {
          strncpy( bufCoarse, tk, 28 );
          bufCoarse[28] = 0;
          coarseExpString.setRaw( bufCoarse );
	}

      }

      else if ( strcmp( tk, "label" ) == 0 ) {

        tk = strtok_r( NULL, "\"\n \t", &context );
        if ( tk ) {
          label.setRaw( tk );
	}

      }

    }

  } while ( more );

  this->initSelectBox(); // call after getting x,y,w,h

  actWin->ci->setRGB( fgR, fgG, fgB, &pixel );
  index = actWin->ci->pixIndex( pixel );
  fgColor.setColorIndex( index, actWin->ci );

  actWin->ci->setRGB( bgR, bgG, bgB, &pixel );
  index = actWin->ci->pixIndex( pixel );
  bgColor.setColorIndex( index, actWin->ci );

  actWin->fi->loadFontTag( fontTag );
  actWin->drawGc.setFontTag( fontTag, actWin->fi );
  fs = actWin->fi->getXFontStruct( fontTag );

  updateDimensions();

  return 1;

}

int activeUpdownButtonClass::genericEdit ( void ) {

char title[32], *ptr;

  ptr = actWin->obj.getNameFromClass( "activeUpdownButtonClass" );
  if ( ptr )
    strncpy( title, ptr, 31 );
  else
    strncpy( title, activeUpdownButtonClass_str2, 31 );

  strncat( title, activeUpdownButtonClass_str3, 31 );

  bufX = x;
  bufY = y;
  bufW = w;
  bufH = h;

  bufFgColor = fgColor.pixelIndex();

  bufBgColor = bgColor.pixelIndex();

  bufTopShadowColor = topShadowColor;
  bufBotShadowColor = botShadowColor;
  strncpy( bufFontTag, fontTag, 63 );

  if ( destPvExpString.getRaw() )
    strncpy( bufDestPvName, destPvExpString.getRaw(), 39 );
  else
    strncpy( bufDestPvName, "", 39 );

  if ( savePvExpString.getRaw() )
    strncpy( bufSavePvName, savePvExpString.getRaw(), 39 );
  else
    strncpy( bufSavePvName, "", 39 );

  if ( fineExpString.getRaw() )
    strncpy( bufFine, fineExpString.getRaw(), 39 );
  else
    strncpy( bufFine, "", 39 );

  if ( coarseExpString.getRaw() )
    strncpy( bufCoarse, coarseExpString.getRaw(), 39 );
  else
    strncpy( bufCoarse, "", 39 );

  if ( label.getRaw() )
    strncpy( bufLabel, label.getRaw(), 39 );
  else
    strncpy( bufLabel, "", 39 );

  buf3D = _3D;
  bufInvisible = invisible;
  bufRate = rate;

  ef.create( actWin->top, actWin->appCtx->ci.getColorMap(),
   &actWin->appCtx->entryFormX,
   &actWin->appCtx->entryFormY, &actWin->appCtx->entryFormW,
   &actWin->appCtx->entryFormH, &actWin->appCtx->largestH,
   title, NULL, NULL, NULL );

  ef.addTextField( activeUpdownButtonClass_str4, 30, &bufX );
  ef.addTextField( activeUpdownButtonClass_str5, 30, &bufY );
  ef.addTextField( activeUpdownButtonClass_str6, 30, &bufW );
  ef.addTextField( activeUpdownButtonClass_str7, 30, &bufH );
  ef.addTextField( activeUpdownButtonClass_str8, 30, bufDestPvName, 39 );
  ef.addTextField( activeUpdownButtonClass_str25, 30, bufSavePvName, 39 );
  ef.addTextField( activeUpdownButtonClass_str9, 30, bufCoarse, 39 );
  ef.addTextField( activeUpdownButtonClass_str10, 30, bufFine, 39 );
  ef.addTextField( activeUpdownButtonClass_str11, 30, &bufRate );
  ef.addToggle( activeUpdownButtonClass_str12, &buf3D );
  ef.addToggle( activeUpdownButtonClass_str13, &bufInvisible );
  ef.addTextField( activeUpdownButtonClass_str14, 30, bufLabel, 39 );
  ef.addColorButton( activeUpdownButtonClass_str16, actWin->ci, &fgCb, &bufFgColor );
  ef.addColorButton( activeUpdownButtonClass_str17, actWin->ci, &bgCb, &bufBgColor );
  ef.addColorButton( activeUpdownButtonClass_str18, actWin->ci, &topShadowCb, &bufTopShadowColor );
  ef.addColorButton( activeUpdownButtonClass_str19, actWin->ci, &botShadowCb, &bufBotShadowColor );

  ef.addFontMenu( activeUpdownButtonClass_str15, actWin->fi, &fm, fontTag );

  XtUnmanageChild( fm.alignWidget() ); // no alignment info

  return 1;

}

int activeUpdownButtonClass::editCreate ( void ) {

  this->genericEdit();
  ef.finished( udbtc_edit_ok, udbtc_edit_apply, udbtc_edit_cancel_delete, this );
  actWin->currentEf = NULL;
  ef.popup();

  return 1;

}

int activeUpdownButtonClass::edit ( void ) {

  this->genericEdit();
  ef.finished( udbtc_edit_ok, udbtc_edit_apply, udbtc_edit_cancel, this );
  actWin->currentEf = &ef;
  ef.popup();

  return 1;

}

int activeUpdownButtonClass::erase ( void ) {

  if ( deleteRequest ) return 1;

  XDrawRectangle( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.eraseGC(), x, y, w, h );

  XFillRectangle( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.eraseGC(), x, y, w, h );

  return 1;

}

int activeUpdownButtonClass::eraseActive ( void ) {

  if ( !init || !activeMode || invisible ) return 1;

  XDrawRectangle( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.eraseGC(), x, y, w, h );

  XFillRectangle( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.eraseGC(), x, y, w, h );

  return 1;

}

int activeUpdownButtonClass::draw ( void ) {

int tX, tY;
XRectangle xR = { x, y, w, h };

  if ( deleteRequest ) return 1;

  actWin->drawGc.saveFg();

  actWin->drawGc.setFG( bgColor.pixelColor() );

  actWin->drawGc.setLineStyle( LineSolid );
  actWin->drawGc.setLineWidth( 1 );

  XFillRectangle( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x, y, w, h );

  if ( !_3D ) {

    actWin->drawGc.setFG( actWin->ci->pix(botShadowColor) );

  }

  XDrawRectangle( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x, y, w, h );

  if ( _3D ) {

  actWin->drawGc.setFG( actWin->ci->pix(botShadowColor) );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x, y, x+w, y );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x, y, x, y+h );

   actWin->drawGc.setFG( actWin->ci->pix(topShadowColor) );

   XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
    actWin->drawGc.normGC(), x, y+h, x+w, y+h );

   XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
    actWin->drawGc.normGC(), x+w, y, x+w, y+h );

  actWin->drawGc.setFG( actWin->ci->pix(topShadowColor) );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+1, y+1, x+w-1, y+1 );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+2, y+2, x+w-2, y+2 );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+1, y+1, x+1, y+h-1 );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+2, y+2, x+2, y+h-2 );

  actWin->drawGc.setFG( actWin->ci->pix(botShadowColor) );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+1, y+h-1, x+w-1, y+h-1 );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+2, y+h-2, x+w-2, y+h-2 );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+w-1, y+1, x+w-1, y+h-1 );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+w-2, y+2, x+w-2, y+h-2 );

  }

  actWin->drawGc.setFG( fgColor.pixelColor() );

  XDrawLine( actWin->d, XtWindow(actWin->drawWidget),
   actWin->drawGc.normGC(), x+5, y+9, x+w-5, y+9 );

  if ( fs ) {

    actWin->drawGc.addNormXClipRectangle( xR );

    actWin->drawGc.setFontTag( fontTag, actWin->fi );

    tX = x + w/2;
    tY = y + h/2 - fontAscent/2;

    if ( label.getRaw() )
      drawText( actWin->drawWidget, &actWin->drawGc, fs, tX, tY,
       XmALIGNMENT_CENTER, label.getRaw() );
    else
      drawText( actWin->drawWidget, &actWin->drawGc, fs, tX, tY,
       XmALIGNMENT_CENTER, "" );

    actWin->drawGc.removeNormXClipRectangle();

  }

  actWin->drawGc.restoreFg();

  return 1;

}

int activeUpdownButtonClass::drawActive ( void ) {

int tX, tY;
char string[63+1];
XRectangle xR = { x, y, w, h };

  if ( !init || !activeMode || invisible ) return 1;

  actWin->executeGc.saveFg();

  actWin->executeGc.setFG( bgColor.getColor() );

  actWin->executeGc.setLineStyle( LineSolid );
  actWin->executeGc.setLineWidth( 1 );

  XFillRectangle( actWin->d, XtWindow(actWin->executeWidget),
   actWin->executeGc.normGC(), x, y, w, h );

  if ( !_3D ) {

    actWin->executeGc.setFG( actWin->ci->pix(botShadowColor) );

  }

  XDrawRectangle( actWin->d, XtWindow(actWin->executeWidget),
   actWin->executeGc.normGC(), x, y, w, h );

  if ( !buttonPressed ) {

    if ( _3D ) {

    actWin->executeGc.setFG( actWin->ci->pix(botShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x, y, x+w, y );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x, y, x, y+h );

    actWin->executeGc.setFG( actWin->ci->pix(topShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x, y+h, x+w, y+h );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+w, y, x+w, y+h );

    // top
    actWin->executeGc.setFG( actWin->ci->pix(topShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+1, y+1, x+w-1, y+1 );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+2, y+2, x+w-2, y+2 );

    // left
    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+1, y+1, x+1, y+h-1 );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+2, y+2, x+2, y+h-2 );

    // bottom
    actWin->executeGc.setFG( actWin->ci->pix(botShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+1, y+h-1, x+w-1, y+h-1 );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+2, y+h-2, x+w-2, y+h-2 );

    // right
    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+w-1, y+1, x+w-1, y+h-1 );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+w-2, y+2, x+w-2, y+h-2 );

    }

  }
  else {

    if ( _3D ) {

    actWin->executeGc.setFG( actWin->ci->pix(botShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x, y, x+w, y );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x, y, x, y+h );

    // top

    actWin->executeGc.setFG( actWin->ci->pix(topShadowColor) );

    // bottom

    actWin->executeGc.setFG( actWin->ci->pix(topShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x, y+h, x+w, y+h );

    //right

    actWin->executeGc.setFG( actWin->ci->pix(topShadowColor) );

    XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
     actWin->executeGc.normGC(), x+w, y, x+w, y+h );

    }

  }

  actWin->executeGc.setFG( fgColor.getColor() );

  XDrawLine( actWin->d, XtWindow(actWin->executeWidget),
   actWin->executeGc.normGC(), x+5, y+9, x+w-5, y+9 );

  if ( fs ) {

    if ( label.getExpanded() )
      strncpy( string, label.getExpanded(), 39 );
    else
      strncpy( string, "", 39 );

    if ( isSaved ) {
      strncat( string, " *", 63 );
    }

    actWin->executeGc.addNormXClipRectangle( xR );

    actWin->executeGc.setFontTag( fontTag, actWin->fi );

    tX = x + w/2;
    tY = y + h/2 - fontAscent/2;

    drawText( actWin->executeWidget, &actWin->executeGc, fs, tX, tY,
     XmALIGNMENT_CENTER, string );

    actWin->executeGc.removeNormXClipRectangle();

  }

  actWin->executeGc.restoreFg();

  return 1;

}

int activeUpdownButtonClass::activate (
  int pass,
  void *ptr )
{

int stat, opStat, n;
Arg args[5];
XmString str;

  switch ( pass ) {

  case 1:

    needConnectInit = needErase = needDraw = needRefresh = 0;
    init = 0;
    aglPtr = ptr;
    opComplete = 0;
    widgetsCreated = 0;
    keyPadOpen = 0;
    isSaved = 0;

#ifdef __epics__
    destEventId = saveEventId = 0;
#endif

    destPvConnected = savePvConnected = active = buttonPressed = 0;
    activeMode = 1;

    incrementTimerValue = (int) ( 1000.0 * rate );
    if ( incrementTimerValue < 50 ) incrementTimerValue = 50;

    if ( !fineExpString.getExpanded() ||
       ( strcmp( fineExpString.getExpanded(), "" ) == 0 ) ) {
      fine = 0;
    }
    else {
      fine = atof( fineExpString.getExpanded() );
    }

    if ( !coarseExpString.getExpanded() ||
       ( strcmp( coarseExpString.getExpanded(), "" ) == 0 ) ) {
      coarse = 0;
    }
    else {
      coarse = atof( coarseExpString.getExpanded() );
    }

    if ( !destPvExpString.getExpanded() ||
       ( strcmp( destPvExpString.getExpanded(), "" ) == 0 ) ) {
      destExists = 0;
    }
    else {
      destExists = 1;
    }

    if ( !savePvExpString.getExpanded() ||
       ( strcmp( savePvExpString.getExpanded(), "" ) == 0 ) ) {
      saveExists = 0;
    }
    else {
      saveExists = 1;
    }

    break;

  case 2:

    if ( !opComplete ) {

      if ( !widgetsCreated ) {

        n = 0;
        XtSetArg( args[n], XmNmenuPost, (XtArgVal) "<Btn5Down>;" ); n++;
        popUpMenu = XmCreatePopupMenu( actWin->topWidgetId(), "", args, n );

        pullDownMenu = XmCreatePulldownMenu( popUpMenu, "", NULL, 0 );

        str = XmStringCreateLocalized( "Save" );
        pbSave = XtVaCreateManagedWidget( "", xmPushButtonWidgetClass,
         popUpMenu,
         XmNlabelString, str,
         NULL );
        XmStringFree( str );

        XtAddCallback( pbSave, XmNactivateCallback, menu_cb,
         (XtPointer) this );

        str = XmStringCreateLocalized( "Restore" );
        pbRestore = XtVaCreateManagedWidget( "", xmPushButtonWidgetClass,
         popUpMenu,
         XmNlabelString, str,
         NULL );
        XmStringFree( str );

        XtAddCallback( pbRestore, XmNactivateCallback, menu_cb,
         (XtPointer) this );

        str = XmStringCreateLocalized( "Set Coarse" );
        pbCoarse = XtVaCreateManagedWidget( "", xmPushButtonWidgetClass,
         popUpMenu,
         XmNlabelString, str,
         NULL );
        XmStringFree( str );

        XtAddCallback( pbCoarse, XmNactivateCallback, menu_cb,
         (XtPointer) this );

        str = XmStringCreateLocalized( "Set Fine" );
        pbFine = XtVaCreateManagedWidget( "", xmPushButtonWidgetClass,
         popUpMenu,
         XmNlabelString, str,
         NULL );
        XmStringFree( str );

        XtAddCallback( pbFine, XmNactivateCallback, menu_cb,
         (XtPointer) this );

        str = XmStringCreateLocalized( "Set Rate (sec)" );
        pbRate = XtVaCreateManagedWidget( "", xmPushButtonWidgetClass,
         popUpMenu,
         XmNlabelString, str,
         NULL );
        XmStringFree( str );

        XtAddCallback( pbRate, XmNactivateCallback, menu_cb,
         (XtPointer) this );

        str = XmStringCreateLocalized( "Set Value" );
        pbValue = XtVaCreateManagedWidget( "", xmPushButtonWidgetClass,
         popUpMenu,
         XmNlabelString, str,
         NULL );
        XmStringFree( str );

        XtAddCallback( pbValue, XmNactivateCallback, menu_cb,
         (XtPointer) this );

	widgetsCreated = 1;

      }

      opStat = 1;

#ifdef __epics__

      if ( destExists ) {
        stat = ca_search_and_connect( destPvExpString.getExpanded(), &destPvId,
         udbtc_monitor_dest_connect_state, this );
        if ( stat != ECA_NORMAL ) {
          printf( activeUpdownButtonClass_str20 );
          opStat = 0;
        }
      }
      else {
        init = 1;
        drawActive();
      }

      if ( saveExists ) {
        stat = ca_search_and_connect( savePvExpString.getExpanded(), &savePvId,
         udbtc_monitor_save_connect_state, this );
        if ( stat != ECA_NORMAL ) {
          printf( activeUpdownButtonClass_str20 );
          opStat = 0;
        }
      }

      if ( opStat & 1 ) opComplete = 1;

#endif

      return opStat;

    }

    break;

  case 3:
  case 4:
  case 5:
  case 6:

    break;

  }

  return 1;

}

int activeUpdownButtonClass::deactivate (
  int pass
) {

int stat;

  if ( pass == 1 ) {

  active = 0;
  activeMode = 0;

  if ( incrementTimerActive ) {
    if ( incrementTimer ) {
      XtRemoveTimeOut( incrementTimer );
      incrementTimer = 0;
    }
    incrementTimerActive = 0;
  }

  if ( 	widgetsCreated ) {
    XtDestroyWidget( popUpMenu );
    widgetsCreated = 0;
  }

  if ( kp.isPoppedUp() ) {
    kp.popdown();
  }

#ifdef __epics__

  if ( destExists ) {
    stat = ca_clear_channel( destPvId );
    if ( stat != ECA_NORMAL )
      printf( activeUpdownButtonClass_str22 );
  }

  if ( saveExists ) {
    stat = ca_clear_channel( savePvId );
    if ( stat != ECA_NORMAL )
      printf( activeUpdownButtonClass_str22 );
  }

#endif

  }

  return 1;

}

void activeUpdownButtonClass::updateDimensions ( void )
{

  if ( fs ) {
    fontAscent = fs->ascent;
    fontDescent = fs->descent;
    fontHeight = fontAscent + fontDescent;
  }
  else {
    fontAscent = 10;
    fontDescent = 5;
    fontHeight = fontAscent + fontDescent;
  }

}

void activeUpdownButtonClass::btnUp (
  int _x,
  int _y,
  int buttonState,
  int buttonNumber,
  int *action )
{

XButtonEvent be;

  *action = 0;

  if ( ( _y - y ) < 10 ) {
    memset( (void *) &be, 0, sizeof(XButtonEvent) );
    be.x_root = actWin->x+_x;
    be.y_root = actWin->y+_y;
    XmMenuPosition( popUpMenu, &be );
    XtManageChild( popUpMenu );
    return;
  }

  if ( !buttonPressed ) return;

  if ( keyPadOpen ) return;

  if ( incrementTimerActive ) {
    if ( incrementTimer ) {
      XtRemoveTimeOut( incrementTimer );
      incrementTimer = 0;
    }
    incrementTimerActive = 0;
  }

  buttonPressed = 0;

//    printf( "btn up\n" );

  actWin->appCtx->proc->lock();
  needRefresh = 1;
  actWin->addDefExeNode( aglPtr );
  actWin->appCtx->proc->unlock();

}

void activeUpdownButtonClass::btnDown (
  int _x,
  int _y,
  int buttonState,
  int buttonNumber,
  int *action )
{

int stat;
double dval;

  *action = 0;

  if ( keyPadOpen ) return;

  if ( ( _y - y ) < 10 ) return;

  buttonPressed = 1;

//    printf( "btn down, x=%-d, y=%-d\n", _x-x, _y-y );

#ifdef __epics__

  actWin->appCtx->proc->lock();
  dval = curControlV;
  needRefresh = 1;
  actWin->addDefExeNode( aglPtr );
  actWin->appCtx->proc->unlock();

  if ( buttonNumber == 3 ) {
    dval += fine;
  }
  else if ( buttonNumber == 1 ) {
    dval -= fine;
  }

#ifdef __epics__
  stat = ca_put( DBR_DOUBLE, destPvId, &dval );
#endif

  if ( buttonNumber == 3 ) {
    incrementTimer = XtAppAddTimeOut( actWin->appCtx->appContext(),
     500, udbtc_increment, this );
    incrementTimerActive = 1;
  }
  else if ( buttonNumber == 1 ) {
    incrementTimer = XtAppAddTimeOut( actWin->appCtx->appContext(),
     500, udbtc_decrement, this );
    incrementTimerActive = 1;
  }

#endif

}

int activeUpdownButtonClass::getButtonActionRequest (
  int *up,
  int *down,
  int *drag,
  int *focus )
{

  *drag = 0;

  if ( destExists )
    *focus = 1;
  else
    *focus = 0;

  if ( !destExists ) {
    *up = 0;
    *down = 0;
    return 1;
  }

  *down = 1;
  *up = 1;

  return 1;

}

int activeUpdownButtonClass::expand1st (
  int numMacros,
  char *macros[],
  char *expansions[] )
{

int stat, retStat = 1;

  stat = destPvExpString.expand1st( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = savePvExpString.expand1st( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = fineExpString.expand1st( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = coarseExpString.expand1st( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = label.expand1st( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;

  return retStat;

}

int activeUpdownButtonClass::expand2nd (
  int numMacros,
  char *macros[],
  char *expansions[] )
{

int stat, retStat = 1;

  stat = destPvExpString.expand2nd( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = savePvExpString.expand2nd( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = fineExpString.expand2nd( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = coarseExpString.expand2nd( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;
  stat = label.expand2nd( numMacros, macros, expansions );
  if ( !( stat & 1 ) ) retStat = stat;

  return stat;

}

int activeUpdownButtonClass::containsMacros ( void ) {

  if ( destPvExpString.containsPrimaryMacros() ) return 1;

  if ( savePvExpString.containsPrimaryMacros() ) return 1;

  if ( fineExpString.containsPrimaryMacros() ) return 1;

  if ( coarseExpString.containsPrimaryMacros() ) return 1;

  if ( label.containsPrimaryMacros() ) return 1;

  return 0;

}

void activeUpdownButtonClass::executeDeferred ( void ) {

int nc, nsc, nd, ne, nr, stat;

  if ( actWin->isIconified ) return;

  actWin->appCtx->proc->lock();
  nc = needConnectInit; needConnectInit = 0;
  nsc = needSaveConnectInit; needSaveConnectInit = 0;
  nd = needDraw; needDraw = 0;
  ne = needErase; needErase = 0;
  nr = needRefresh; needRefresh = 0;
  actWin->remDefExeNode( aglPtr );
  actWin->appCtx->proc->unlock();

//----------------------------------------------------------------------------

#ifdef __epics__

  if ( nc ) {

    destPvConnected = 1;
    destType = ca_field_type( destPvId );

    stat = ca_add_masked_array_event( destType, 1, destPvId,
     udbtc_controlUpdate, (void *) this, (float) 0.0, (float) 0.0, (float) 0.0,
     &destEventId, DBE_VALUE );
    if ( stat != ECA_NORMAL )
      printf( activeUpdownButtonClass_str23 );

    bgColor.setConnected();

    init = 1;
    drawActive();

  }

  if ( nsc ) {

    savePvConnected = 1;
    saveType = ca_field_type( savePvId );

    stat = ca_add_masked_array_event( saveType, 1, savePvId,
     udbtc_saveUpdate, (void *) this, (float) 0.0, (float) 0.0, (float) 0.0,
     &saveEventId, DBE_VALUE );
    if ( stat != ECA_NORMAL )
      printf( activeUpdownButtonClass_str23 );

  }

#endif

//----------------------------------------------------------------------------

  if ( nd ) {

    drawActive();

  }

//----------------------------------------------------------------------------

  if ( ne ) {

    eraseActive();

  }

//----------------------------------------------------------------------------

  if ( nr ) {

    eraseActive();
    drawActive();

  }

//----------------------------------------------------------------------------

}

char *activeUpdownButtonClass::firstDragName ( void ) {

  dragIndex = 0;
  return dragName[dragIndex];

}

char *activeUpdownButtonClass::nextDragName ( void ) {

  return NULL;

}

char *activeUpdownButtonClass::dragValue (
  int i ) {

  return destPvExpString.getExpanded();

}

void activeUpdownButtonClass::changeDisplayParams (
  unsigned int _flag,
  char *_fontTag,
  int _alignment,
  char *_ctlFontTag,
  int _ctlAlignment,
  char *_btnFontTag,
  int _btnAlignment,
  int _textFgColor,
  int _fg1Color,
  int _fg2Color,
  int _offsetColor,
  int _bgColor,
  int _topShadowColor,
  int _botShadowColor )
{

  if ( _flag & ACTGRF_TEXTFGCOLOR_MASK )
    fgColor.setColorIndex( _textFgColor, actWin->ci );

  if ( _flag & ACTGRF_BGCOLOR_MASK )
    bgColor.setColorIndex( _bgColor, actWin->ci );

  if ( _flag & ACTGRF_TOPSHADOWCOLOR_MASK )
    topShadowColor = _topShadowColor;

  if ( _flag & ACTGRF_BOTSHADOWCOLOR_MASK )
    botShadowColor = _botShadowColor;

  if ( _flag & ACTGRF_BTNFONTTAG_MASK ) {
    strncpy( fontTag, _btnFontTag, 63 );
    fontTag[63] = 0;
    actWin->fi->loadFontTag( fontTag );
    fs = actWin->fi->getXFontStruct( fontTag );
    updateDimensions();
  }

}

void activeUpdownButtonClass::changePvNames (
  int flag,
  int numCtlPvs,
  char *ctlPvs[],
  int numReadbackPvs,
  char *readbackPvs[],
  int numNullPvs,
  char *nullPvs[],
  int numVisPvs,
  char *visPvs[],
  int numAlarmPvs,
  char *alarmPvs[] )
{

  if ( flag & ACTGRF_CTLPVS_MASK ) {
    if ( numCtlPvs ) {
      destPvExpString.setRaw( ctlPvs[0] );
    }
  }

}

#ifdef __cplusplus
extern "C" {
#endif

void *create_activeUpdownButtonClassPtr ( void ) {

activeUpdownButtonClass *ptr;

  ptr = new activeUpdownButtonClass;
  return (void *) ptr;

}

void *clone_activeUpdownButtonClassPtr (
  void *_srcPtr )
{

activeUpdownButtonClass *ptr, *srcPtr;

  srcPtr = (activeUpdownButtonClass *) _srcPtr;

  ptr = new activeUpdownButtonClass( srcPtr );

  return (void *) ptr;

}

#ifdef __cplusplus
}
#endif