__author__ = 'Carsten Agerskov'

import ConfigParser
import gdata.gauth

config = ConfigParser.RawConfigParser()
config.read('PBMonitor.config')

clientId = config.get('Google','clientId')
clientSecret = config.get('Google','clientSecret')
redirectURI = config.get('Google','redirectURI')
userAgent = config.get('Google','userAgent')

clientScope='https://spreadsheets.google.com/feeds/'

token = gdata.gauth.OAuth2Token(client_id=clientId,client_secret=clientSecret,scope=clientScope,user_agent=userAgent)
print "Open the URL below to get the verification code"
print token.generate_authorize_url(redirect_uri=redirectURI)
code = raw_input('What is the verification code? ').strip()
token.get_access_token(code)
print "Refresh token: ", token.refresh_token
config.set('Google','refreshToken',token.refresh_token)
print "Access Token: ", token.access_token

with open('PBMonitor.config', 'wb') as configfile:
    config.write(configfile)
