import PowerCycle

__author__ = 'Carsten Agerskov'

import time

import gdata.spreadsheets.client
import gdata.gauth
import ConfigParser
from PowerCycle import PowerCycle


def powerCycle2GSheet(queue):
    print "powerCycle2GSheet started"

    config = ConfigParser.RawConfigParser()
    config.read('PBMonitor.config')

    clientId = config.get('Google','clientId')
    clientSecret = config.get('Google','clientSecret')
    userAgent = config.get('Google','userAgent')
    refreshToken = config.get('Google','refreshToken')
    gSheetKey = config.get('Google', 'gSheetKey')

    clientScope='https://spreadsheets.google.com/feeds/'

    token = gdata.gauth.OAuth2Token(client_id=clientId, client_secret=clientSecret, scope=clientScope, user_agent=userAgent, refresh_token=refreshToken)
    client = gdata.spreadsheets.client.SpreadsheetsClient()
    token.authorize(client)

    entry = client.get_list_feed(gSheetKey, 'od6').entry[0]

    powerCycle = PowerCycle()

    dict = ''
    while dict != 'PBMonShutDown':
        dict = queue.get()

        if dict != 'PBMonShutDown':
            print "Read powerCycle from queue: ", dict
            for t in dict:
                entry.set_value(t.lower(),str(dict[t]))

            retry = True
            while retry:
                try:
                    client.add_list_entry(entry,gSheetKey,'od6')
                    print "Data sent to GSheet"
                    retry = False
                except:
                    retry = True
                    time.delay(60)
                    print "Transmission to GSheet failed, retrying in 60 seconds"





