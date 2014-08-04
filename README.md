MCIVU
=====

Keyboard Protocol
----------------------------------------
        CMD_READY - key pressed
         CMD_MODE - switch between PIPELINE and STAGE modes
       CMD_DEPLOY - redeploy a selected STAGE or PIPELINE
    CMD_DEPLOY_TO - deploy to the NEXT STAGE in a pipeline
         CMD_BACK - scroll back through a stage or pipeline depending on current mode
      CMD_FORWARD - scroll forward through a stage or pipeline depending on current mode


Display Protocol
----------------------------------------
    Format: DSP:LN{1|2}:[SCR:][COL{R|Y|G}|CMX-{[R|G|B]}*]:TEXT

    Legend:
       DSP - Display command
       LN1 - LCD line 1 display
       LN2 - LCD line 2 display
       SCR - Scroll text on screen
       COL - LED colour indicator
       CMX - Colour matrix indicating colour to display for each LED
      TEXT - Text to display
         R - Red colour
         Y - Yellow colour
         G - Green colour


- **Example 1:**
DSP:LN1:COLG:SAMPLE TEXT 1
Display the text 'SAMPLE TEXT 1' text on line 1 and turn the 1st LED indicator red.
    
- **Example 2:**
DSP:LN2:SCR:SAMPLE TEXT 2
Scroll the text 'SAMPLE TEXT 2' on line 2
    
- **Example 3:**
DSP:LN1:SCR:CMX-GGGY:SAMPLE TEXT 3
Scroll the text 'SAMPLE TEXT 3' on line 1, turn the 1st, 2nd, 3rd and 4th LEDs Green, Green, Green and Yellow respectively
