/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* Copyright (C) 2012 by Maciej Gajewski <maciej.gajewski0@gmail.com>
* All rights reserved.
*
* The Original Code is: all of this file.
*/

#include "Synchronizer.hpp"

#include <QMutexLocker>

namespace Turtle {

Synchronizer::Synchronizer()
    : _semaphore(nullptr)
{
}

void Synchronizer::Arm()
{
    QMutexLocker l(&_mutex);

    Q_ASSERT(_semaphore == nullptr);
    _semaphore = new QSemaphore(0);
}

void Synchronizer::Unlock()
{
    QMutexLocker l(&_mutex);
    Q_ASSERT(_semaphore);
    _semaphore->release(1);
}

void Synchronizer::UnlockIfArmed()
{
    QMutexLocker l(&_mutex);
    if(_semaphore)
        _semaphore->release(1);
}

void Synchronizer::WaitAndUnarm()
{
    {
        QMutexLocker l(&_mutex);
        Q_ASSERT(_semaphore);
    }
    _semaphore->acquire(1); // this will wait until semaphore has been released

    {
        QMutexLocker l(&_mutex);
        delete _semaphore;
    }

}

} // namespace Turtle
