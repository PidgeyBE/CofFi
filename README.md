# CofFi
Controlling Delonghi Coffee machine using a servo, activated by smartphone app

Used technologies:
- ESP8266 for making connection with cloud and activating servo (Programmed via Arduino IDE)
- PHP webserver for keeping track of all coffees made in a MySQL database. When a user requests a new coffee via the app, a new entry is created in the database. This new entry will be discovered by the ESP8266, which will start making the coffee.
- "HTTTP Widget" app from Google Play store to call the minimal API on my webserver (https://play.google.com/store/apps/details?id=net.rosoftlab.httpwidget1&hl=en)
