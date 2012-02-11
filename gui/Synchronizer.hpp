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
#ifndef TURTLE_SYNCHRONIZER_HPP
#define TURTLE_SYNCHRONIZER_HPP

#include <QSemaphore>
#include <QMutex>

namespace Turtle {

// Implementas the synchronization logic needed to synchronize interpeter an ui threads
// in a clean way, to cach concurrency issues
class Synchronizer
{
public:
    Synchronizer();

    void Arm(); // arms the device
    void Unlock(); // can be called only once when device is armed
    void UnlockIfArmed(); // canm be called on unarmed instance
    void WaitAndUnarm(); // waits until the device is unlocked, unarms

private:

    QMutex _mutex;
    QSemaphore* _semaphore;

};

} // namespace Turtle

#endif // TURTLE_SYNCHRONIZER_HPP
