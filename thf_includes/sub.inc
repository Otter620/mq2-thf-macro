
SUB isHolyShit
	/if (${Me.Combat} && !${Me.Casting.ID} && !${Cursor.ID}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB isDownShit
	/if (!${Me.Combat} && !${Me.Casting.ID} && !${Cursor.ID}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB instantBuff
	/declare i int local
	/declare myInstantList string local ${Ini[${iniSetup},InstantBuff]}
	/declare myInstantListCount int local ${myInstantList.Count[|]}
	/varcalc myInstantListCount ${myInstantListCount}-1
	/declare myInstantLine string local
	/declare myInstant string local
	/declare mySlot string local
	/declare myInstantSpell string local

	/if (${myInstantListCount} > 0) {
		/for i 1 to ${myInstantListCount}
			/varset mySlot ${myInstantList.Token[${i},|]}
			/varset myInstantLine ${Ini[${iniSetup},InstantBuff,${mySlot}]}
			|- en 1, l'item
			/varset myInstant ${myInstantLine.Token[1,/]}
			/varset myInstantSpell ${FindItem[=${myInstant}].Spell}
			/if (${Spell[${myInstantSpell}].Stacks} && !${Me.Buff[${myInstantSpell}].ID}) {
				/casting "${myInstant}" ${mySlot}
			}
		/next i
	}
/RETURN

SUB checkSpellBuff
	/declare i int local
	/declare targetID int local
	/declare mySpellList string local ${Ini[${iniSetup},SpellBuff]}
	/declare mySpellListCount int local
	/varcalc mySpellListCount ${mySpellList.Count[|]}-1
	/declare mySpellLine string local
	/declare mySpell string local
	/declare myGem string local
	/declare targetName string local

	/if (${mySpellListCount} > 0) {
		/for i 1 to ${mySpellListCount}
			/varset myGem ${mySpellList.Token[${i},|]}
			/varset mySpellLine ${Ini[${iniSetup},SpellBuff,${myGem}]}
			/varset mySpell ${mySpellLine.Token[1,/]}
			/varset targetName ${mySpellLine.Token[2,/]}
			|- Si on a une target pour le spell, c'est que ce n'est pas un group spell
			/if (${targetName.Length}) {
				/varset targetID ${Spawn[pc ${targetName} los radius ${Spell[${mySpell}].MyRange}].ID}
				/if (${Me.CurrentMana} > ${Spell[${mySpell}].Mana} && ${Me.SpellReady[${mySpell}]} && ${targetID} && !${Me.Moving}) {
					/call checkTargetNeedBuff ${targetName} ${Spell[${mySpell}].ID}
					/if (${Macro.Return}) {
						/if (${targetID}) {
							/chat ${channelNumber} casting "${mySpell}" on [${targetName}]
							/casting "${mySpell}" ${myGem} -maxtries|3 -targetid|${Spawn[pc ${targetName}].ID}
							/delay 3s
						}
					}
				}
			} else {
				/if (${Spell[${mySpell}].Mana} > ${Me.CurrentMana} && ${Spell[${mySpell}].Stacks} && !${Me.Moving}) {
					|- Verifier la presence de tous les membres du groupe avant de caster le bazar
					/call checkGroupNeedBuff ${Spell[${mySpell}].ID}
					/if (${Macro.Return}) {
						/chat ${channelNumber} casting "${mySpell}"
						/casting "${mySpell}" ${myGem} -maxtries|3
					}
				}
			}
		/next i
	}
/RETURN

SUB checkTargetNeedBuff(string targetName, int spellID)
	/declare i int local
	/declare targetBuffs string local
	/declare targetBuff	string local
	
	/varset targetBuffs ${NetBots[${targetName}].Buff}
	/if (${targetBuffs.Find[${spellID}]}) {
		/RETURN FALSE
	}
	/for i 1 to ${targetBuffs.Count[ ]} - 1
		/varset targetBuff ${targetBuffs.Token[${i}, ]}
		/if (${targetBuff.Length}) {
			/if (!${Spell[${spellID}].WillStack[${Spell[${targetBuff}]}]}) {
				/chat ${channelNumber} '${Spell[${spellID}].Name}' won't stack with '${Spell[${targetBuff}]}' !
				/RETURN FALSE
			}
		}
	/next i
/RETURN TRUE

SUB checkGroupNeedBuff(int spellID)

/RETURN FALSE

SUB checkDiscipline
	/declare i int local
	/declare myDiscList string local ${Ini[${iniSetup},Discipline]}
	/declare myDiscListCount int local
	/varcalc myDiscListCount ${myDiscList.Count[|]}-1
	/declare myDisc string local
	/declare myNumber string local
	
	/if (${myDiscListCount} > 0) {
		/for i 1 to ${myDiscListCount}
			/varset myNumber ${myDiscList.Token[${i},|]}
			/varset myDisc ${Ini[${iniSetup},Discipline,${myNumber}]}
			/if (${Me.CombatAbilityReady[${myDisc}]} && ${Target.PctHPs}<99) {
				/disc ${myDisc}
			}
		/next i
	}
/RETURN

SUB checkHolyShitBuff
	/declare i int local
	/declare j int local
	/declare myHSBList string local ${Ini[${iniSetup},HolyShitBuff]}
	/declare myHSBListCount int local ${myHSBList.Count[|]}
	/varcalc myHSBListCount ${myHSBListCount}-1
	/declare myHSBLine string local
	/declare myHSB string local |- HolyShitBuff
	/declare myHSS string local |- HolyShitSpell
	/declare mySlot string local
	/declare recastTime string local
	/declare itemCleanName string local
	/declare targetName string local
	/declare NewString string local

	/if (${myHSBListCount} > 0) {
		/for i 1 to ${myHSBListCount}
			/varset mySlot ${myHSBList.Token[${i},|]}
			/varset myHSBLine ${Ini[${iniSetup},HolyShitBuff,${mySlot}]}
			|- en 1, l'item
			/varset myHSB ${myHSBLine.Token[1,/]}
			|- en 2, le timer
			/varset recastTime ${myHSBLine.Token[2,/]}
			|- en 3, la target
			/varset targetName ${myHSBLine.Token[3,/]}
			/varset itemCleanName ${myHSB.Replace[ ,].Replace[',].Replace[.,]}
			/if (${itemCleanName.Find[,]}) {
				/varset NewString
				/for j 1 to ${itemCleanName.Length}
					/if (${itemCleanName.Mid[${j},1].NotEqual[,]}) {
						/varset NewString ${NewString}${itemCleanName.Mid[${j},1]}
					}
				/next j
				/varset itemCleanName ${NewString}
			}
			/if (!${Defined[${itemCleanName}Timer]}) /declare ${itemCleanName}Timer timer outer 0
			/varset myHSS ${FindItem[=${myHSB}].Spell}
			/if (${Spell[${myHSS}].Stacks} && !${Me.Buff[${myHSS}].ID} && ${${itemCleanName}Timer} <= 0 && !${Me.Moving}) {
				/echo Casting "${myHSB}" ${mySlot}
				/casting "${myHSB}" ${mySlot}
				/varset ${itemCleanName}Timer ${recastTime}
			} else {
				/if (${${itemCleanName}Timer} <= 0 && !${Me.Moving} && ${itemCleanName.Lower.Equal[artoriastheabysswalker]}) {
					/echo Casting "${myHSB}" ${mySlot}
					/casting "${myHSB}" ${mySlot}
					/varset ${itemCleanName}Timer ${recastTime}
				}
			}
		/next i
	}
/RETURN

SUB checkDownShitBuff
	/declare i int local
	/declare myDSBList string local ${Ini[${iniSetup},DownShitBuff]}
	/declare myDSBListCount int local ${myDSBList.Count[|]}
	/varcalc myDSBListCount ${myDSBListCount}-1
	/declare myDSBLine string local
	/declare myDSB string local |- DownShitBuff
	/declare myDSS string local |- DownShitSpell
	/declare mySlot string local
	/declare recastTime string local
	/declare itemCleanName string local
	/declare targetName string local

	/if (${myDSBListCount} > 0) {
		/for i 1 to ${myDSBListCount}
			/varset mySlot ${myDSBList.Token[${i},|]}
			/varset myDSBLine ${Ini[${iniSetup},DownShitBuff,${mySlot}]}
			|- en 1, l'item
			/varset myDSB ${myDSBLine.Token[1,/]}
			|- en 2, le timer
			/varset recastTime ${myDSBLine.Token[2,/]}
			|- en 3, la target
			/varset targetName ${myDSBLine.Token[3,/]}
			/varset itemCleanName ${myDSB.Replace[ ,].Replace[',].Replace[.,]}
			/if (!${Defined[${itemCleanName}Timer]}) /declare ${itemCleanName}Timer timer outer 0
			/varset myDSS ${FindItem[=${myDSB}].Spell}
			/if (${Spell[${myDSS}].Stacks} && !${Me.Buff[${myDSS}].ID} && ${${itemCleanName}Timer} <= 0 && !${Me.Moving}) {
				/casting "${myDSB}" ${mySlot} -targetid|${Spawn[pc ${targetName}].ID}
				/varset ${itemCleanName}Timer ${recastTime}
				/delay 3s
			}
		/next i
	}
/RETURN

SUB EVENT_Dead
	|/call Wait4Rez
	/ssm max
/RETURN

SUB EVENT_Camp
	/echo Camping...
	/end
/RETURN

SUB EVENT_Hail(String Line, String Cible)
	/declare toTheZone string local
	/varset toTheZone ${Ini[${iniHail},General,${Cible.Lower}]}
	/if (${toTheZone.Length} && ${toTheZone.NotEqual[NULL]}) {
		/say ${toTheZone}
	} else {
		/echo ${Line}
	}
	/RETURN

SUB EVENT_Task(String Line, String Tache)
	/tell ${myMain} ${Line}
/RETURN

SUB EVENT_Assign(String Line, String Tache)
	/tell ${myMain} ${Line}
/RETURN

SUB checkPlugIn
	/if (!${Plugin[MQ2Cast].Name.Length}) {
		/echo MQ2Cast is a REQUIRED PLUGIN
		/echo Please Load MQ2Cast before running this macro
		/end
	}
/RETURN

SUB EVENT_Tell(Line, string From, string Msg)
	/if (${From.Find[${myMain}]}) {
		/if (${Msg.Equal[loot]}) {
			/call LootMobs
		}
		/if (${Msg.Lower.Find[find].NotEqual[NULL]}) {
			
		}
	} else {
		/if (${Me.CleanName.NotEqual[${myMain}]}) {
			/chat ${channelNumber} ${Line}
		}
	}
/RETURN

SUB EVENT_Group(Line, string From)
	/if (${From.Find[${myMain}]}) {
		/if (!${Me.Grouped}) {
			/invite
		}
	}
/RETURN

SUB EVENT_DraekonVoice(string Line, string targetName)
	/moveto id ${Spawn[npc ${targetName}].ID}
/RETURN


SUB EVENT_DraekonVoiceEnd(string Line, string targetName)
	/moveto id ${Spawn[npc Draekon].ID}
/RETURN


SUB EVENT_DraekonNoGrass
	/if (${Me.Class.Name.Equal[Warrior]} || ${Me.Class.Name.Equal[Paladin]} || ${Me.Class.Name.Equal[Shadowknight]}) /RETURN

	/declare EndX float local			-1609.88
	/declare EndY float local			308.23

	/call InternalMoveTo ${EndY} ${EndX}
/RETURN


SUB EVENT_DraekonGrass
	/moveto id ${Spawn[npc Draekon].ID}
/RETURN


SUB EVENT_DraekonBelly
	/moveto id ${Spawn[npc Draekon].ID}
/RETURN

SUB InternalMoveTo(int y, int x)

	/declare currentX int local 0
	/declare currentY int local 0
	/declare currentZ int local 0
	/moveto loc ${y} ${x}
:waitUntilMovingFinished
	/if (${Math.Distance[${y},${x},${Me.Z}]} > 15 || ${Math.Distance[${currentY},${currentX},${currentZ}]} > 5) {
		/varset currentX ${Me.X}
		/varset currentY ${Me.Y}
		/varset currentZ ${Me.Z}
		/delay 1
		/doevents
		/goto :waitUntilMovingFinished
	}

/RETURN

SUB isOneOfMyToons(String from)
	/if (${NetBots.Client.Find[${from}]}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB cleanToonName(String toonName)
	/if (${NetBots.Client.Find[${toonName}]}) {
		/RETURN ${toonName}
	}
	/if (${NetBots.Client.Find[${toonName.Right[-2].Left[-1]}]}) {
		/RETURN ${toonName.Right[-2].Left[-1]}
	}
/RETURN ${toonName}

SUB sticking(String to)
    /if (${Spawn[${to}].ID} == ${Me.ID}) {
        /RETURN
    }
	/call stopCurrentAction FALSE
    /target ${to}
    /face fast
    /delay 1s
    /stick ${outerStickCommand}
    /chat ${channelNumber} Sticking to ${to}
/RETURN

|- Allow toon to gate to the Forest
SUB gate(String from, String who)
	/echo gate
    /call amIconcerned ${from} ${who}
    /if (${Macro.Return}) {
        /call stopCurrentAction TRUE
        /declare CharmOfQueenAdrianne string local ${FindItem[Charm of Queen Adrianne].Name}
        /if (${CharmOfQueenAdrianne.Length} > 0 && !${CharmOfQueenAdrianne.Equal[NULL]}) {
            /casting "${CharmOfQueenAdrianne}" charm
            /chat ${channelNumber} Gating to the Forest !
        }
    }
/RETURN

|- stick off, attack off, interrupt, twist off (if specify)
SUB stopCurrentAction(bool evenBard)
	/echo stopCurrentAction ${evenBard}
    /if (${evenBard}) {
        /twist off
    }
	/target clear
    /stick off
    /attack off
    /interrupt
    /delay 1
/RETURN

SUB amIMelee
	/if (${Select[${Me.Class.ShortName},BRD,BER,BST,MNK,PAL,RNG,ROG,SHD,WAR]}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB amINuker
	/if (${Select[${Me.Class.ShortName},WIZ,MAG,ENC,NEC]}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB amIPetClass
	/if (${Select[${Me.Class.ShortName},BST,MAG,NEC]}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB amIconcerned(String from, String who)
	/echo amIconcerned ${from} ${who}
	/if (${who.Equal[all]}) {
		/RETURN TRUE
	}
	/if (${who.Equal[group]}) {
		/if (${Me.Grouped} && ${Group.Member[${from}]}) {
			/RETURN TRUE
		}
	}
	/if (${who.Equal[caster]}) {
		|- Can cast spells, including Bard 
		/if (${Me.Class.CanCast}) {
			/RETURN TRUE
		}
	}
	/if (${who.Equal[melee]}) {
		|- Monk/Rogue/Bard/Ranger/Warrior/Paladin/ShadowKnight?
		/call amIMelee
		/RETURN ${Macro.Return}
	}
	/if (${who.Equal[priest]}) {
		|- Cleric/Druid/Shaman? 
		/if (${Me.Class.HealerType}) {
			/RETURN TRUE
		}
	}
	/if (${who.Equal[nuker]}) {
		|- Wizard/Magician/Necromancer/Enchanter?
		/call amINuker
		/RETURN ${Macro.Return}
	}
	/if (${who.Equal[${Me.Class.Name}]} || ${who.Equal[${Me.Class.ShortName}]}) {
		/RETURN TRUE
	}
	/if (${who.Equal[${Me.CleanName}]}) {
		/RETURN TRUE
	}
/RETURN FALSE

SUB makeGroup
	/if (${Me.CleanName.Equal[conradd]}) {
		/invite tiarra
		/delay 1
		/invite tifania
		/delay 1
		/invite brocelia
		/delay 1
		/invite gaya
		/delay 1
		/invite houdi
		/RETURN
	}
	/if (${Me.CleanName.Equal[vivyana]}) {
		/invite arsen
		/delay 1
		/invite aiguisee
		/delay 1
		/invite fandril
		/delay 1
		/invite shoalin
		/delay 1
		/invite korale
		/RETURN
	}
	/if (${Me.CleanName.Equal[namea]}) {
		/invite margona
		/delay 1
		/invite mirlen
		/delay 1
		/invite elmunster
		/delay 1
		/invite malefica
		/delay 1
		/invite aede
		/RETURN
	}
/RETURN

SUB LPS
	|- ---------------------------------------------------------------------
	|- Declare loops / second variables
	|- ---------------------------------------------------------------------
	/if (!${Defined[looptest]}) 	/declare looptest 	int outer 0
	/if (!${Defined[loops]}) 		/declare loops 		int outer 0
	/if (!${Defined[looptime]}) 	/declare looptime 	int outer ${Time.Second}
	/if (!${Defined[loopsecmax]}) 	/declare loopsecmax int outer 0
	/if (!${Defined[loopsecmin]}) 	/declare loopsecmin int outer 10000
	/if (!${Defined[loopcount]}) 	/declare loopcount 	int outer 0

	/if (${Time.Second}>=${Math.Calc[${looptime}+1]}) {
		/if (${looptest}>${loopsecmax}) /varset loopsecmax ${looptest}
		/if (${looptest}<${loopsecmin} && ${looptest}) /varset loopsecmin ${looptest}
		/varset loops ${looptest}
		/varset looptest 0
		/varset looptime ${Time.Second}
	} else /if (${Time.Second}==0 && ${looptime}!=0) {
		/echo LPS:${looptest} loop/sec min:${loopsecmin} max:${loopsecmax}
		/varset looptest 0
		/varset looptime 0
		/varset loopsecmax 0
		/varset loopsecmin 10000
	}
/RETURN

|- Allow toon to the guild hall
SUB goToGuildHall(String from, String who)
	/call gate ${from} ${who}
    /call amIconcerned ${from} ${who}
    /if (${Macro.Return}) {
        /call stopCurrentAction TRUE
        /declare CharmOfQueenAdrianne string local ${FindItem[Charm of Queen Adrianne].Name}
        /if (${CharmOfQueenAdrianne.Length} > 0 && !${CharmOfQueenAdrianne.Equal[NULL]}) {
            /casting "${CharmOfQueenAdrianne}" charm
            /chat ${channelNumber} Gating to the Forest !
        }
    }
/RETURN

SUB test()
	|- Fermer toutes les fenetres ouvertes d'abord !
	/notify InventoryWindow InvSlot13 rightmouseheld
	/delay 1
	/keypress Enter
	/shiftkey /keypress . chat
	/nomodkey /keypress c chat
	/nomodkey /keypress h chat
	/nomodkey /keypress q chat
	/nomodkey /keypress t chat
	/nomodkey /keypress space chat
	/shiftkey /keypress ${channel} chat
	/keypress Space chat
	/delay 1
	/notify ItemDisplayWindow IconButton leftmouseup
	/delay 1
	/keypress Enter chat
	/delay 1
	/notify ItemDisplayWindow 0 close 
/RETURN

SUB endMacro
	/deletevar * global
	/disband
	/raiddisband
	/end
/RETURN

