#!/usr/bin/env python
# -*- coding: utf-8 -*-
from telegram.ext import Updater, CommandHandler, MessageHandler, Filters
import telegram
import logging
import time
import datetime
import random
from dateutil import tz
import rospy 
from std_msgs.msg import String 


humidity = ""
heading = ""
pressure = ""
temperature = ""
air_q = ""
dust = ""

# Enable logging
logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                    level=logging.INFO)

logger = logging.getLogger(__name__)


# Define a few command handlers. These usually take the two arguments bot and
# update. Error handlers also receive the raised TelegramError object in error.

def start (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('Welcome to Neko Sat')


def temp (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The Temperature is: ' + temperature + '°C')


def humidity_u (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The Humidity: ' + humidity + '%')


def heading_u (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The Heading is: ' + heading + '°')

def pressure_u (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The pressure is: ' + pressure + ' Pa')

def air_q_u (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The air quality is: ' + air_q + '%')


def dust_u (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The dust levels are ' + dust)

def alt_u (bot, update):
    bot.send_chat_action(chat_id=update.message.chat_id, action=telegram.ChatAction.TYPING) # Added this to make it more natural
    update.message.reply_text('The altitude is ' + alt[:-7] + "m.")




def error(bot, update, error):
    """Log Errors caused by Updates."""
    logger.warning('Update "%s" caused error "%s"', update, error)


def callback (data):
    global humidity
    global heading 
    global pressure 
    global temperature 
    global air_q
    global dust 
    global alt
    try:
        humidity, heading ,pressure, temperature,air_q,dust,alt = data.data.split(",")
    except:
        humidity, heading ,pressure, temperature,air_q,dust,alt = ["Error","Error","Error","Error","Error","Error","Error"]

def main():
    """Start the bot."""
    # Create the EventHandler and pass it your bot's token.
    updater = Updater("584806957:AAG6HWWWNJr2xa3umhVk4MW1LH7TLSjWQTM")


    # Get the dispatcher to register handlers
    dp = updater.dispatcher

    # on different commands - answer in Telegram
    dp.add_handler(CommandHandler("temperature", temp))
    dp.add_handler(CommandHandler("heading", heading_u))
    dp.add_handler(CommandHandler("pressure", pressure_u))
    dp.add_handler(CommandHandler("air_quality", air_q_u))
    dp.add_handler(CommandHandler("dust", dust_u))
    dp.add_handler(CommandHandler("humidity", humidity_u))
    dp.add_handler(CommandHandler("altitude", alt_u))
    dp.add_handler(CommandHandler("start", start))

    # log all errors
    dp.add_error_handler(error)

    # Start the Bot
    updater.start_polling()

    # Run the bot until you press Ctrl-C or the process receives SIGINT,
    # SIGTERM or SIGABRT. This should be used most of the time, since
    # start_polling() is non-blocking and will stop the bot gracefully.
    rospy.init_node('neko_cat', anonymous=True)

    rospy.Subscriber('data_topic', String, callback)

    updater.idle()
    rospy.spin()


if __name__ == '__main__':
    main()

