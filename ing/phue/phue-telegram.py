#!/usr/bin/env python
# -*- coding: utf-8 -*-
# pylint: disable=W0613, C0116
# type: ignore[union-attr]
# This program is dedicated to the public domain under the CC0 license.

"""
Simple Bot to send timed Telegram messages.

This Bot uses the Updater class to handle the bot and the JobQueue to send
timed messages.

First, a few handler functions are defined. Then, those functions are passed to
the Dispatcher and registered at their respective places.
Then, the bot is started and runs until we press Ctrl-C on the command line.

Usage:
Basic Alarm Bot example, sends a message after a set time.
Press Ctrl-C on the command line or send a signal to the process to stop the
bot.
"""

import logging
import picamera
import time
from telegram import Update
from telegram.ext import Updater, CommandHandler, CallbackContext
from phue import Bridge

# Enable logging
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO
)

logger = logging.getLogger(__name__)

def rgb_to_xy(red, green, blue):
    # gamma correction
    red = pow((red + 0.055) / (1.0 + 0.055), 2.4) if red > 0.04045 else (red / 12.92)
    green = pow((green + 0.055) / (1.0 + 0.055), 2.4) if green > 0.04045 else (green / 12.92)
    blue =  pow((blue + 0.055) / (1.0 + 0.055), 2.4) if blue > 0.04045 else (blue / 12.92)

    # convert rgb to xyz
    x = red * 0.649926 + green * 0.103455 + blue * 0.197109
    y = red * 0.234327 + green * 0.743075 + blue * 0.022598
    z = green * 0.053077 + blue * 1.035763

    # convert xyz to xy
    x = x / (x + y + z)
    y = y / (x + y + z)

    # TODO check color gamut if known

    return [x, y]


b = Bridge('192.168.7.154')
b.connect()

lights = b.lights

# Define a few command handlers. These usually take the two arguments update and
# context. Error handlers also receive the raised TelegramError object in error 
def start(update: Update, context: CallbackContext) -> None:
    update.message.reply_text('Hi! we have 4 command!')
    update.message.reply_text("You can turn on Philips hue : /turn on <light's index(0, 1, 2)>")
    update.message.reply_text("Also, you can turn off Philips hue : /turn off <light's index(0, 1, 2)>")
    update.message.reply_text("You can change light's color : /color <light's index> <red, blue, green>")
    update.message.reply_text("Othere way : /color <light's index> <3 color codes(0~1)>")
    update.message.reply_text("example : /color 1 0.5 0.5 0.5")

def alarm(context):
    """Send the alarm message."""
    time.sleep(1)
    job = context.job
    context.bot.send_message(job.context, text='Beep!')


def remove_job_if_exists(name, context):
    """Remove job with given name. Returns whether job was removed."""
    current_jobs = context.job_queue.get_jobs_by_name(name)
    if not current_jobs:
        return False
    for job in current_jobs:
        job.schedule_removal()
    return True

def turn_on(update: Update, context: CallbackContext) -> None:
    chat_id = update.message.chat_id
    
    try:
        choose = int(context.args[0])
        if light_index_error(choose):
            update.message.reply_text('You can choose 0, 1, 2!!')
            return
        
        if lights[choose].on:
            update.message.reply_text('Already turned on!!!')
            return

        lights[choose].on = True
        update.message.reply_text('Turned on {0}!'.format(choose))
    except():
        update.message.reply_text('Usage: /turn_on <0,1,2>')

def turn_off(update: Update, context: CallbackContext) -> None:
    chat_id = update.message.chat_id

    try:
        choose = int(context.args[0])
        if light_index_error(choose):
            update.message.reply_text('You can choose 0, 1, 2!!')
            return

        if lights[choose].on == False:
            update.message.reply_text('Already turned off!!!')
            return

        lights[choose].on = False
        update.message.reply_text('Turned off {0}!'.format(choose))
    except():
        update.message.reply_text('Usage: /trun_off <0,1,2>')

def color(update: Update, context: CallbackContext) -> None:
    chat_id = update.message.chat_id
    input_color = list()
    xy_color = list()

    try:
        choose = int(context.args[0])

        if light_index_error(choose):
            update.message.reply_text('First argument must be <0,1,2>')
            return

        if context.args[1] == 'red':
            input_color = [1,0,0]
        elif context.args[1] == 'green':
            input_color = [0,1,0]
        elif context.args[1] == 'blue':
            input_color = [0,0,1]
        else:
            input_color.append(float(context.args[1]))
            input_color.append(float(context.args[2]))
            input_color.append(float(context.args[3]))

        if light_color_error(input_color[0], input_color[1], input_color[2]):
            update.message.reply_text('2nd, 3rd, 4th argument must be 0-1')
            return

        xy_color = rgb_to_xy(input_color[0], input_color[1], input_color[2])
        lights[choose].xy = xy_color
        update.message.reply_text('Changed It!')
    except Exception as e:
        print(e)
        update.message.reply_text('Usage: /color <index> <color1> <color2> <color3> or /color <index> red/blue/green')

def light_index_error(choose):
    if choose<0 or choose>2:
        return 1
    else:
        return 0

def light_color_error(color1, color2, color3):
    if color1<0 or color1>1 or color2<0 or color2>1 or color3<0 or color3>1:
        return 1
    else:
        return 0

def set_timer(update: Update, context: CallbackContext) -> None:
    """Add a job to the queue."""
    chat_id = update.message.chat_id
    try:
        # args[0] should contain the time for the timer in seconds
        due = int(context.args[0])
        if due < 0:
            update.message.reply_text('Sorry we can not go back to future!')
            return

        job_removed = remove_job_if_exists(str(chat_id), context)
        context.job_queue.run_once(alarm, due, context=chat_id, name=str(chat_id))

        text = 'Timer successfully set!'
        if job_removed:
            text += ' Old one was removed.'
        update.message.reply_text(text)

    except (IndexError, ValueError):
        update.message.reply_text('Usage: /set <seconds>')


def unset(update: Update, context: CallbackContext) -> None:
    """Remove the job if the user changed their mind."""
    chat_id = update.message.chat_id
    job_removed = remove_job_if_exists(str(chat_id), context)
    text = 'Timer successfully cancelled!' if job_removed else 'You have no active timer.'
    update.message.reply_text(text)


def main():
    """Run bot."""
    # Create the Updater and pass it your bot's token.
    # Make sure to set use_context=True to use the new context based callbacks
    # Post version 12 this will no longer be necessary
    updater = Updater("1442656694:AAHtA5CTqU7V2qRwXHh7s2GM7cSEr3qBbjI", use_context=True)

    # Get the dispatcher to register handlers
    dispatcher = updater.dispatcher

    # on different commands - answer in Telegram
    dispatcher.add_handler(CommandHandler("start", start))
    dispatcher.add_handler(CommandHandler("help", start))
    dispatcher.add_handler(CommandHandler("set", set_timer))
    dispatcher.add_handler(CommandHandler("unset", unset))
    dispatcher.add_handler(CommandHandler('turn_on', turn_on))
    dispatcher.add_handler(CommandHandler('turn_off', turn_off))
    dispatcher.add_handler(CommandHandler('color', color))

    # Start the Bot
    updater.start_polling()

    # Block until you press Ctrl-C or the process receives SIGINT, SIGTERM or
    # SIGABRT. This should be used most of the time, since start_polling() is
    # non-blocking and will stop the bot gracefully.
    updater.idle()


if __name__ == '__main__':
    main()
