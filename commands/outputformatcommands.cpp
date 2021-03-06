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

#include "outputformatcommands.h"

/** related to adding a new Output Format */

AddNewOutputFormatCommand::AddNewOutputFormatCommand(OutputFormat *newFormat, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), parser(parser)
{
  this->newFormat = new OutputFormat(* newFormat);
  setText(QObject::tr("Create new outputformat '%1'").arg(newFormat->getName()));
}

void AddNewOutputFormatCommand::undo(void) {
  parser->removeOutputFormat(newFormat);
}

void AddNewOutputFormatCommand::redo(void) {
  parser->addOutputFormat(newFormat);
}

AddNewOutputFormatCommand::~AddNewOutputFormatCommand() {
  delete newFormat;
}

/** related to removing an Output Format */

RemoveOutputFormatCommand::RemoveOutputFormatCommand(OutputFormat *fmtToRemove, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), parser(parser)
{
  this->fmtToRemove = new OutputFormat(* fmtToRemove);
  setText(QObject::tr("Remove outputformat '%1'").arg(fmtToRemove->getName()));
}

void RemoveOutputFormatCommand::undo(void) {
  parser->addOutputFormat(fmtToRemove);
}

void RemoveOutputFormatCommand::redo(void) {
  parser->removeOutputFormat(fmtToRemove);
}

RemoveOutputFormatCommand::~RemoveOutputFormatCommand() {
  delete fmtToRemove;
}

/** related to modifying an existing Output Format */

UpdateOutputFormatCommand::UpdateOutputFormatCommand(OutputFormat *fmtToUpdate, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), fmtToUpdate(new OutputFormat(* fmtToUpdate)), parser(parser)
{
  originalFmt = parser->getOutputFormat(fmtToUpdate->getOriginalName());
  setText(QObject::tr("Update outputformat '%1'").arg(fmtToUpdate->getName()));
}

void UpdateOutputFormatCommand::undo(void) {
  // Note: What if a command adds a fmt which has the same name ?
  parser->updateOutputFormat(originalFmt);
}

void UpdateOutputFormatCommand::redo(void) {
  parser->updateOutputFormat(fmtToUpdate);
}

UpdateOutputFormatCommand::~UpdateOutputFormatCommand() {
  // both objects are managed by the QUndo command
  delete fmtToUpdate;
  delete originalFmt;
}


/**  related to setting the default outputformat */

SetDefaultOutputFormatCommand::SetDefaultOutputFormatCommand(QString const & s, MapfileParser *parser, QUndoCommand *parent)
     : QUndoCommand(parent), newDefaultOf(s), parser(parser)
{
  oldDefaultOf = parser->getDefaultOutputFormat();
  setText(QObject::tr("set default output format to '%1'").arg(newDefaultOf));
}

void SetDefaultOutputFormatCommand::undo(void) {
  parser->setDefaultOutputFormat(oldDefaultOf);
}

void SetDefaultOutputFormatCommand::redo(void) {
  parser->setDefaultOutputFormat(newDefaultOf);
}

