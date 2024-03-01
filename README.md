This project was a method to gauge Magic the Gathering deck strength when I was unable to play against others.

Having time during Covid I went through my card collection and made many decks to give away or use to teach people but could not test them against each other to see what decks worked on a level playing field.

To find out that metric I created a C++ that would take a deck list and play a simple game. I used how fast the deck could play all cards in an opening hand as a metric. I used the rule that the 'player' would play the biggest spells they could in their hand each turn, no matter if it was from the opening hand.

The program understood Lands (one per turn), mana pools, mana cost, card draw, and tapping/untapping.

The program was set aside when I deemed inputting the deck list for 80+ decks to be too much work for the desired outcome.

There is a bunch more pseudo-code I could make real code for this program so that it could do such things as combat phase and casting non-permanent spells.

