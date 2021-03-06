/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#ifndef OUTPUTFORMATCOMMANDS_H
#define OUTPUTFORMATCOMMANDS_H

#include <QUndoCommand>

#include "../parser/mapfileparser.h"

class AddNewOutputFormatCommand : public QUndoCommand {

 public:
   AddNewOutputFormatCommand(OutputFormat * newOf, MapfileParser * parser, QUndoCommand *parent = 0);
   ~AddNewOutputFormatCommand();
   void undo();
   void redo();

 private:
   OutputFormat * newFormat;
   MapfileParser * parser;
};

class RemoveOutputFormatCommand : public QUndoCommand {

 public:
   RemoveOutputFormatCommand(OutputFormat * fmtToRemove, MapfileParser * parser, QUndoCommand *parent = 0);
   ~RemoveOutputFormatCommand();
   void undo();
   void redo();

 private:
   OutputFormat * fmtToRemove;
   MapfileParser * parser;
};

class UpdateOutputFormatCommand : public QUndoCommand {

 public:
   UpdateOutputFormatCommand(OutputFormat * fmtToUpdate, MapfileParser * parser, QUndoCommand *parent = 0);
   ~UpdateOutputFormatCommand();
   void undo();
   void redo();

 private:
   OutputFormat * fmtToUpdate, * originalFmt;
   MapfileParser * parser;
};

class SetDefaultOutputFormatCommand : public QUndoCommand {

 public:
   SetDefaultOutputFormatCommand(QString const & newDefaultOf, MapfileParser * parser, QUndoCommand *parent = 0);
   void undo();
   void redo();

 private:
   QString oldDefaultOf, newDefaultOf;
   MapfileParser * parser;
};

#endif // OUTPUTFORMATCOMMANDS_H
