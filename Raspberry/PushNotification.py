__author__ = 'agerscar'

"""
    PushNotification: For pushing notifications
    Copyright (C) 2015  Carsten Agerskov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import ConfigParser
from Instapush import Instapush, App
import datetime
import time

def pushNotification(queue):
    print "pushNotification started"
    config = ConfigParser.RawConfigParser()
    config.read('PBMonitor.config')

    applicationId = config.get('Instapush','applicationId')
    applicationSecret =  config.get('Instapush','applicationSecret')
    pushApp = App(appid=applicationId, secret=applicationSecret)

    n = ('',)
    while n[0] != 'PBMonShutDown':
        n = queue.get()
        retry = True
        while retry:
            try:
                print "pushing notification", n
                pushApp.notify(event_name=n[0], trackers={ 'State': n[1], 'DateTime': n[2]})
                retry = False
            except:
                retry = True
                print "Push of notification failed, retrying in 60 seconds"
                time.sleep(60)
            else:
                retry = False

