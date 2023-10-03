# ArkSlugs

Arkslugs is a simulator game that uses Random Number Generator, TFT LCD, and
Servo Motor to play the game. The aim of this game is to raise your slugs to rank S and
it can change the appearance. It also has battle moments to raise your slug to a higher
rank. You use the joystick to do actions which include training, feeding, and playing.
You can all of these actions to raise your slugs. The 16x2 LCD screen gives 4 different actions for you to take. 
The actions are: feed, play, train, and stats. 

The game mechanic is to raise your own Slug by training while you maintain the energy of the Slug. You can do actions on your Slug such as feed, play, and train. 
Feed is an action that can regenerate Slug's energy by 50 no matter what. 
Train is an action that can increase the attack and hp of the Slug by consuming energy of Slug. There is a chance that it can increase both of these stats by double or you can fail your train. 
Play is an action that can regenerate a little bit of energy, increase Slug's hp, and attack a little bit. There is a chance that it can regenerate the energy by double. 
Stats is an action that can see your Slug's hp, attack, energy and rank. 

The goal of this simulator is to raise your Slug to have an S rank, starting from D. To raise your rank, you need to win a match against the opponent Slug. The match happens after 5 turns and the turns are consumed by using the action once except Stats. The servo motor tells you whether you won the match or not by flipping it to right or left. If it turns left, it means your Slug won the match and it increases your Slug's rank. If you lose this match, nothing will change and you will be able to do Slug's training again until the match begins in 5 turns. 

I used SPI libraries for the TFT LCD component to make transferring the data easier and coding commands for it. 
I also used the LiquidCrystal library to control the 16x2 LCD to print the messages and actions on the screen. 

Wiring Diagram:
![Screenshot 2022-12-07 104752](https://github.com/despair0098/ArkSlugs/assets/22960474/47512b31-3892-4e53-b5d5-a842bdd9267c)

How the code works in terms of State Machine Diagrams:
![IMG_2165](https://github.com/despair0098/ArkSlugs/assets/22960474/2310ddd0-a4a8-4814-862e-fe497a199e51)
![IMG_2166](https://github.com/despair0098/ArkSlugs/assets/22960474/b0606fca-3c60-4e6a-9814-86980cb58b5a)
![IMG_2167](https://github.com/despair0098/ArkSlugs/assets/22960474/16e4e148-3f42-4857-9d2b-25f691cf07f1)
![IMG_2164](https://github.com/despair0098/ArkSlugs/assets/22960474/3b87c715-10cb-4c70-bfbb-1610b26a245d)







