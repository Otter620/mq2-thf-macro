| v2.1

Sub Event_Chat(string ChatType, string Sender, string ChatText)
	/echo ${ChatType}, ${Sender}, ${ChatText}
	/return
	/if (!${ChatType.Equal[TELL]} || ${Sender.Equal[${Me.CleanName}]} || !${Spawn[${Sender}].ID}) /return
	/if (${Sender.Find[${strMainTank}]} && ${ChatText.Equal[Loot]}) /call LootMobs
/return


Sub Event_ChatSetup(Line, string ChatText)
	/echo ${ChatText}
	/varset chat ${ChatText}
	/echo !!! >>> ChatSetup [ ${chat} ] done <<< !!!
/return


Sub Event_ChatDebuffSetup(Line, string ChatDebuffText)
	/varset chatdebuff ${ChatDebuffText}
	/echo !!! >>> ChatDebuffSetup [ ${chatdebuff} ] done <<< !!!
/return


Sub Event_Itemcheck(Line, string OutChannel, string Item)
	/if (${FindItemCount[=${Item}]}>0 || ${FindItemBankCount[=${Item}]}>0) {
		/if (${FindItemCount[=${Item}]}>0) /${OutChannel} FOUND (${FindItemCount[=${Item}]}) ${Item} in inventory
		/if (${FindItemBankCount[=${Item}]}>0) /${OutChannel} FOUND (${FindItemBankCount[=${Item}]}) ${Item} in bank
	}
	/if (${FindItemCount[=${Item}]}<=0 && ${FindItemBankCount[=${Item}]}<=0) /${OutChannel} << NOT >> found ${Item}
/return


Sub Event_ValikStart
	/if (${Me.Class.Name.Equal[Wizard]}) /varset EventValik GO
/return


Sub ValikCheck

	/if (${Me.Class.Name.Equal[Wizard]} && ${EventValikTimer}<=0) {

		/if (${Spawn[Valik the First Creation].ID} && !${Spawn[Valik the first creation].LineOfSight}) {
			/face loc 2000,70 fast nolook
			/bc [Event] - Running to Nuke Spot
			/moveto loc 21.00 -74.00
			/delay 10
			/face loc -2000,70 fast nolook
			
		}
	}

/return


Sub Event_ValikSave

	/if (${Me.Class.Name.Equal[Wizard]}) {

		/if (${Spawn[Valik the First Creation].ID} && ${Spawn[Valik the first creation].LineOfSight}) {
			/face loc -2000,70 fast nolook
			/interrupt
			/bc [Event] - Running to Save Spot
			/moveto loc -21.00 -74.00
		}
		/varset EventValikTimer 10s
	}

/return


Sub Event_KhasvaStart

	/if (!${Defined[DispelTimer]})	/declare DispelTimer timer outer 0
	/if (!${Defined[AlertTimer]})	/declare AlertTimer timer outer 0

	/echo [${Time.Time24}] Khasva event has begun...
	/varset EventKhasva GO
	/varset DispelTimer 450

/return


Sub KhasvaCheck

	/if (${Defined[DispelTimer]} && ${DispelTimer}<51) {

		/if (${DispelTimer}==50 && ${AlertTimer}==0) {
			/if (${broadcast}==1) /${broadcastTell} Dispel happening in 5.0 seconds!
			/varset AlertTimer 3
		}
		/if (${DispelTimer}==40 && ${AlertTimer}==0) {
			/if (${broadcast}==1) /${broadcastTell} Dispel happening in 4.0 seconds!
			/varset AlertTimer 3
		}
		/if (${DispelTimer}==30 && ${AlertTimer}==0) {
			/if (${broadcast}==1) /${broadcastTell} Dispel happening in 3.0 seconds!
			/varset AlertTimer 3
		}
		/if (${DispelTimer}==20 && ${AlertTimer}==0) {
			/if (${broadcast}==1) /${broadcastTell} Dispel happening in 2.0 seconds!
			/varset AlertTimer 3
		}
		/if (${DispelTimer}==10 && ${AlertTimer}==0) {
			/if (${broadcast}==1) /${broadcastTell} Dispel happening in 1.0 seconds!
			/varset AlertTimer 3
		}
		/if (${DispelTimer}<2 && ${AlertTimer}==0) {
			/if (${broadcast}==1) /${broadcastTell} Dispel happening NOW!
			/delay 1
			/varset DispelTimer 450
		}
	}

/return


Sub Event_KhasvaAdds

	/target id ${Spawn[npc Khasva].ID}
	/if (${broadcast}==1) /${broadcastTell} [${Time.Time24}] Khasva is at ${Spawn[npc Khasva].PctHPs}. Adds spawned! Found ${SpawnCount[=Deathwalker Vampire]} of them.
  
/return


Sub Event_KhasvaStrength

	/echo [1] Red
	/noparse /bcaa //if (${Math.Distance[-53.6,71.9,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_RED;/timed 10 /nomodkey /keypress u;/timed 10 /say Clicking Red
	/echo [2] Green
	/noparse /bcaa //if (${Math.Distance[-53,-70.75,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_GREEN;/timed 20 /nomodkey /keypress u;/timed 20 /say Clicking Green
	/echo [3] Yellow
	/noparse /bcaa //if (${Math.Distance[-78.8,-57,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_YELLOW;/timed 30 /nomodkey /keypress u;/timed 30 /say Clicking Yellow
	/echo [4] Blue
	/noparse /bcaa //if (${Math.Distance[-77.5,56.4,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_BLUE;/timed 40 /nomodkey /keypress u;/timed 40 /say Clicking Blue
	/echo [5] Purple
	/noparse /bcaa //if (${Math.Distance[-50.1,0.3,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_PURPLE;/timed 50 /nomodkey /keypress u;/timed 50 /say Clicking Purple
/return

Sub Event_KhasvaAgony
	/echo [1] Yellow
	/noparse /bcaa //if (${Math.Distance[-78.8,-57,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_YELLOW;/timed 10 /nomodkey /keypress u;/timed 10 /say Clicking Yellow
	/echo [2] Purple
	/noparse /bcaa //if (${Math.Distance[-50.1,0.3,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_PURPLE;/timed 20 /nomodkey /keypress u;/timed 20 /say Clicking Purple
	/echo [3] Green
	/noparse /bcaa //if (${Math.Distance[-53,-70.75,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_GREEN;/timed 30 /nomodkey /keypress u;/timed 30 /say Clicking Green
	/echo [4] Red
	/noparse /bcaa //if (${Math.Distance[-53.6,71.9,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_RED;/timed 40 /nomodkey /keypress u;/timed 40 /say Clicking Red
	/echo [5] Blue
	/noparse /bcaa //if (${Math.Distance[-77.5,56.4,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_BLUE;/timed 50 /nomodkey /keypress u;/timed 50 /say Clicking Blue
/return


Sub Event_KhasvaBlood
	/echo [1] Green
	/noparse /bcaa //if (${Math.Distance[-53,-70.75,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_GREEN;/timed 10 /nomodkey /keypress u;/timed 10 /say Clicking Green
	/echo [2] Red
	/noparse /bcaa //if (${Math.Distance[-53.6,71.9,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_RED;/timed 20 /nomodkey /keypress u;/timed 20 /say Clicking Red
	/echo [3] Purple
	/noparse /bcaa //if (${Math.Distance[-50.1,0.3,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_PURPLE;/timed 30 /nomodkey /keypress u;/timed 30 /say Clicking Purple
	/echo [4] Blue
	/noparse /bcaa //if (${Math.Distance[-77.5,56.4,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_BLUE;/timed 40 /nomodkey /keypress u;/timed 40 /say Clicking Blue
	/echo [5] Yellow
	/noparse /bcaa //if (${Math.Distance[-78.8,-57,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_YELLOW;/timed 50 /nomodkey /keypress u;/timed 50 /say Clicking Yellow
/return


Sub Event_KhasvaDread
	/echo [1] Purple
	/noparse /bcaa //if (${Math.Distance[-50.1,0.3,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_PURPLE;/timed 10 /nomodkey /keypress u;/timed 10 /say Clicking Purple
	/echo [2] Yellow
	/noparse /bcaa //if (${Math.Distance[-78.8,-57,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_YELLOW;/timed 20 /nomodkey /keypress u;/timed 20 /say Clicking Yellow
	/echo [3] Red
	/noparse /bcaa //if (${Math.Distance[-53.6,71.9,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_RED;/timed 30 /nomodkey /keypress u;/timed 30 /say Clicking Red
	/echo [4] Blue
	/noparse /bcaa //if (${Math.Distance[-77.5,56.4,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_BLUE;/timed 40 /nomodkey /keypress u;/timed 40 /say Clicking Blue
	/echo [5] Green
	/noparse /bcaa //if (${Math.Distance[-53,-70.75,${Me.Z}]} < 15) /multiline ; /doortarget OBJ_NERIAK_LAMP_GREEN;/timed 50 /nomodkey /keypress u;/timed 50 /say Clicking Green
/return


Sub Event_Zenurix(string Line, string targetName)

	/declare EndX float local			-1050.85
	/declare EndY float local			1969.45
	
	/varset StartX ${Spawn[pc ${targetName}].X}
	/varset StartY ${Spawn[pc ${targetName}].Y}

	/popup ----- ${targetName} -----
	
	/if (${targetName.Equal[${Me.CleanName}]}) {
		/bc [Event] - Running to Safe Spot
		/varset intZenurixCallout 1
		/call InternalMoveTo ${EndY} ${EndX}

	}

/return


Sub Event_ZenurixOFF

	/if (${intZenurixCallout}==1) {
		/bc [Event] - Returning to Raid
		/varset intZenurixCallout 0
		/call InternalMoveTo ${StartY} ${StartX}
	}

/return


Sub Event_DraekonVoice(string Line, string targetName)

	/moveto id ${Spawn[npc ${targetName}].ID}

/return


Sub Event_DraekonVoiceEnd(string Line, string targetName)

	/moveto id ${Spawn[npc Draekon].ID}

/return


Sub Event_DraekonNoGrass

	/if (${Me.Class.Name.Equal[Warrior]} || ${Me.Class.Name.Equal[Paladin]} || ${Me.Class.Name.Equal[Shadow Knight]}) /return

	/declare EndX float local			-1609.88
	/declare EndY float local			308.23

	/call InternalMoveTo ${EndY} ${EndX}

/return


Sub Event_DraekonGrass

	/moveto id ${Spawn[npc Draekon].ID}

/return


Sub Event_DraekonBelly

	/moveto id ${Spawn[npc Draekon].ID}

/return


Sub checkTwins

	/if (${Me.CleanName.Equal[${strTwins1}]} || ${Me.CleanName.Equal[${strTwins2}]}) {

		/if (${Target.CleanName.Equal[${npcEmpress}]}) {
			/bc Empress|${Target.PctHPs}
			/delay 10
		}
	
		/if (${Target.CleanName.Equal[${npcDuke}]}) {
			/bc Duke|${Target.PctHPs}	
			/delay 10
		}

	}

	/if (${Target.CleanName.Equal[${npcEmpress}]}) {	

		/if (${Target.PctHPs} >= ${npcDukeHealth}) {

			/if (${timerEventPopup}<=0) {
				/popup ATTACKING: Empress:${Target.PctHPs}% -- Duke:${npcDukeHealth}%
				/varset timerEventPopup 15
			}
			/varset eventTwinsAttack 1
		} else {
			/if (${timerEventPopup}<=0) {
				/popup PAUSING: Empress:${Target.PctHPs}% -- Duke:${npcDukeHealth}%
				/varset timerEventPopup 15
			}
			/varset eventTwinsAttack 0
		}
	}

	/if (${Target.CleanName.Equal[${npcDuke}]}) {

		/if (${Target.PctHPs} >= ${npcEmpressHealth}) {

			/if (${timerEventPopup}<=0) {
				/popup ATTACKING: Duke:${Target.PctHPs}% -- Empress:${npcEmpressHealth}%
				/varset timerEventPopup 15
			}
			/varset eventTwinsAttack 1
		} else {
			/if (${timerEventPopup}<=0) {
				/popup PAUSING: Duke:${Target.PctHPs}% -- Empress:${npcEmpressHealth}%
				/varset timerEventPopup 15
			}
			/varset eventTwinsAttack 0
		}
	}

/return


Sub Event_DruidEpicOff

	/if (${broadcast}==1) /${broadcastTell} OFF Druid Epic "Ancient Frozen Staff of Brambles"

/return


Sub Event_DruidDonorOff

	/if (${broadcast}==1) /${broadcastTell} OFF Druid Donor "Cerythi, Nature's Spirit"

/return


Sub Event_RemoveCurse

	/if (${Cast.Ready[Galadriel's Gift]} && ${Me.State.Equal[STAND]}) {
		/casting "Galadriel's Gift" item
		/if (${broadcast}==1) /${chat} !! Curse !! Casting "Galadriel's Gift"
		/delay 5
		/return
	}

	/if (${Cast.Ready[${spellRemoveGreaterCurse}]} && ${Me.State.Equal[STAND]} && !${Me.Moving}) {

			/if (${Target.ID}) /declare OldTarget int local ${Target.ID}
			/delay 5
			/if (${Target.ID}!=${Me.ID}) {
				/target id ${Me.ID}
				/delay 20 ${Target.Name.Equal[${Me.Name}]}
			}
			/casting ${spellRemoveGreaterCurse}
			/if (${broadcast}==1) /${chat} !! Curse !! Casting "Spell: RGC" on [ ${Target.CleanName} ]
			/delay 70
			/delay 20 ${Cast.Ready}
			/if (${Spawn[${OldTarget}].ID}) {
				/if (${Target.ID}!=${Spawn[${OldTarget}].ID}) /squelch /target id ${OldTarget}
			} else {
				/squelch /target clear
			}
			/return

	} else /if (${Cast.Ready[Shissar Empire Badge of Honor]} && ${Me.State.Equal[STAND]} && !${Me.Moving}) {

			/if (${Me.Class.Name.Equal[Bard]} && ${Twist.Twisting}) /twist off
			/if (${Target.ID}) /declare OldTarget int local ${Target.ID}
			/delay 5
			/if (${Target.ID}!=${Me.ID}) {
				/target id ${Me.ID}
				/delay 20 ${Target.Name.Equal[${Me.Name}]}
			}
			/casting "Shissar Empire Badge of Honor" item
			/if (${broadcast}==1) /${chat} !! Curse !! Casting "Shissar Empire Badge of Honor" on [ ${Target.CleanName} ]
			/delay 70
			/delay 20 ${Cast.Ready}
			/if (${Spawn[${OldTarget}].ID}) {
				/if (${Target.ID}!=${Spawn[${OldTarget}].ID}) /squelch /target id ${OldTarget}
			} else {
				/squelch /target clear
			}
			/return		
	}

/return


Sub Event_Mana

	/if (${ManaCheckTimer}==0) /bc [Manacheck]
	
	/if (${Me.Class.Name.Equal[Bard]} && ${Twist.Twisting}) /twist off

	/if (${Cast.Ready} && ${Me.State.Equal[STAND]}) {
	
		/if (${Cast.Ready[Fire Crystal Wristguard]}) {
			/if (${broadcast}==1) /${chat} Casting "Fire Crystal Wristguard"
			/casting "Fire Crystal Wristguard" item
			/delay 5
			/return
		}

		/if (${MANACLICKtimer3}==0 && !${Me.Moving}) {

			/if (${Cast.Ready[Immaculate Glowing Black Stone]}) {
				/casting "Immaculate Glowing Black Stone" item
				/if (${broadcast}==1) /${chat} Casting "Immaculate Glowing Black Stone"			
				/varset MANACLICKtimer3 241s
				/delay 5
				/return

			} else /if (${Cast.Ready[Flawless Pearl]}) {
				/casting "Flawless Pearl" item
				/if (${broadcast}==1) /${chat} Casting "Flawless Pearl"
				/varset MANACLICKtimer3 350s
				/delay 5
				/return
				
			} else /if (${Cast.Ready[Entropic Orb of Lithluran]}) {
				/casting "Entropic Orb of Lithluran" item
				/if (${broadcast}==1) /${chat} Casting "Entropic Orb of Lithluran"
				/varset MANACLICKtimer3 241s
				/delay 5
				/return

			} else /if (${Cast.Ready[Powerful Silk Slippers]}) {
				/casting "Powerful Silk Slippers" item
				/if (${broadcast}==1) /${chat} Casting "Powerful Silk Slippers"
				/varset MANACLICKtimer3 301s
				/delay 5
				/return

			} else /if (${Cast.Ready[Silver Chains of the Burgerz]}) {
				/casting "Silver Chains of the Burgerz" item
				/if (${broadcast}==1) /${chat} Casting "Silver Chains of the Burgerz"
				/varset MANACLICKtimer3 451s
				/delay 5
				/return

			} else /if (${Cast.Ready[Glowing Chains]}) {
				/casting "Glowing Chains" item
				/if (${broadcast}==1) /${chat} Casting "Glowing Chains"
				/varset MANACLICKtimer3 451s
				/delay 5
				/return		

			} else /if (${Cast.Ready[Silver Chains]}) {
				/casting "Silver Chains" item
				/if (${broadcast}==1) /${chat} Casting "Silver Chains"
				/varset MANACLICKtimer3 451s
				/delay 5
				/return

			} else /if (${Cast.Ready[Orb of Spirits]}) {
				/casting "Orb of Spirits" item
				/if (${broadcast}==1) /${chat} Casting "Orb of Spirits"
				/varset MANACLICKtimer3 301s
				/delay 5
				/return
			}
		}

		/if (${MANACLICKtimer15}==0 && !${Me.Moving}) {

			/if (${Cast.Ready[Tears of Frozen Spirits]}) {
				/casting "Tears of Frozen Spirits" item
				/if (${broadcast}==1) /${chat} Casting "Tears of Frozen Spirits"
				/varset MANACLICKtimer15 601s
				/delay 10
				/return

			} else /if (${Cast.Ready[Tears of Frozen Mana]}) {
				/casting "Tears of Frozen Mana" item
				/if (${broadcast}==1) /${chat} Casting "Tears of Frozen Mana"
				/varset MANACLICKtimer15 601s
				/delay 10
				/return
			}
		}
	}
	/varset ManaCheckTimer 10s

/return


Sub Event_BuffSune(Line,Sender)

	/varset SuneType GO

/return


Sub Event_BuffFinals(Line,Sender)

	/varset FinalsType GO

/return


Sub Event_RaidInvite(Line,Sender)

	/if (${Raid.Leader.Name.Equal[${Me.CleanName}]}) {
			/delay 5
			/raidinvite ${Sender.Right[-2].Left[-1]}
	}

/return


Sub Event_ResurrectionToon(Line, RezToon, CastingToon)

|#Event ResurrectionToon   		"#*#rezz #1# #2#"
|
|/gsay Testline1: ${Param0}, ${Param1}, ${Param2}, ${Param3}
|/gsay Testline2: 0${Param0}0, 1${Param1}1, 2${Param2}2, 3${Param3}3
|/gsay Testline3: |${Param0}|, |${Param1}|, |${Param2}|, |${Param3}|
|
|/dealy 30
|/return
|
|	/if (${Me.CleanName.Equal[${RezToon}]}) {
|		/varset AcceptType GO
|		/consent ${CastingToon}
|		/gsay Consent ${CastingToon}
|	}
|/gsay 	${Me.CleanName}
|
|			/if (${RezToon.Equal[${Me.CleanName}]}) {
|	 /gsay Test 2
|	}
|			/if (${CastingToon.Equal[${Me.CleanName}]}) {
|	 /gsay Test 3
|	}
|
|	/target "${Sender}'s corpse"
|	/delay 3s ${Target.CleanName.Equal[${Sender}'s corpse]}
|	
|	/if (${Target.CleanName.NotEqual[${Sender}'s corpse]}) /return
|	
|	
|	/if (${Me.Class.Name.Equal[Cleric]}) {
|	/delay damit Rezztoon zuerst consent geben kann
|	/delay 5
|	/gsay test
|		/delay 100 ${Cast.Ready}
|		/squelch /target ${RezToon}'s Corpse
|		/delay 20 ${Target.ID}
|		/if (${Target.Type.Equal[Corpse]}) {
|			/corpse
|			/delay 5
|			/corpse
|			/if (${Cast.Ready[Staff of the Earthcrafter]} && ${Me.State.Equal[STAND]} && !${Me.Moving}) {
|				/casting "Staff of the Earthcrafter" item
|				/if (${broadcast}==1) /${chat} Casting "Staff of the Earthcrafter"
|			} else /tell ${Sender.Right[-2].Left[-1]} !! Could NOT rez the corpse (reason item) !!
|		} else /tell ${Sender.Right[-2].Left[-1]} !! Could NOT rez the corpse (reason corpse)!!
|	}
|
/return


Sub Event_NoSpawn

	/keypress esc
	/if (${Me.Class.Name.Equal[Shaman]}) 		/varset TargetDebuffID 					0
	/if (${Me.Class.Name.Equal[Magician]}) 		/varset TargetDebuffID 					0
	/if (${Me.Class.Name.Equal[Necromancer]}) 	/varset TargetDebuffID 					0
	/if (${Me.Class.Name.Equal[Enchanter]}) 	/varset TargetDebuffID 					0
	/if (${Me.Class.Name.Equal[Druid]}) 		/varset TargetDebuffID 					0
	/if (${Me.Class.Name.Equal[Wizard]})		/varset TargetWizardID 					0
	/if (${Me.Class.Name.Equal[Necromancer]})	/varset TargetNecromancerID 			0
	/if (${Me.Class.Name.Equal[Monk]})			/varset MeleeTargetID 					0
	/if (${Me.Class.Name.Equal[Rogue]})			/varset MeleeTargetID 					0
	/if (${Me.Class.Name.Equal[Bard]})			/varset MeleeTargetID 					0
	/if (${Me.Class.Name.Equal[Berserker]})		/varset MeleeTargetID 					0
	/if (${Me.Class.Name.Equal[Ranger]})		/varset MeleeTargetID 					0
	/if (${Me.Class.Name.Equal[Cleric]} || ${Me.Class.Name.Equal[Druid]} || ${Me.Class.Name.Equal[Shaman]})	/if (${HealTarget.NotEqual[0]}) /varset HealTarget ${Me.ID}
	/if (${Me.Class.Name.Equal[Warrior]} || ${Me.Class.Name.Equal[Rogue]} || ${Me.Class.Name.Equal[Monk]}) /return

/return


Sub Event_Distracted

	/delay 10

/return


Sub Event_Stand

	/stand up

/return



Sub Event_Restart

	/delay 10
	/macro main.mac

/return


Sub Event_Reset

	/varset CothRepeat 0
	/varset CothMode 0
	/varset CothTarget 0
	/varset CothType 0
	/varset CheckPetSummon 0
	/varset PetWeaponID 0
	/varset POTType 0
	/varset SupType 0
	/varset TargetSupID 0
	/varset HealTarget 0
	/varset HealType 0
	/varset HealOthers 0
	/varset TargetDebuffID 0
	/varset TargetNecromancerID 0
	/varset TargetWizardID 0
	/varset MeleeTarget 0
	/varset MeleeTargetID 0
	/varset TankTarget 0
	/varset MaxDPS 0
	/varset	AOETarget 0
	/varset ForageType 0
	/varset	FollowName 0
	/varset CombatMoveCheck 0
	/varset	EventValik 0
	
|	/deletevar * global
	/varset MacroStartTimer 50

/return


Sub Event_Died
	/delay 20 !${Zone.ID}
	/delay 100 ${Zone.ID}
	/delay 5
	/if (${broadcast}==1) /bc >>> ** ${Me.Name} died ** <<<
	/varset SpellsType GO
	/varset AcceptType GO
/return

Sub Event_Finish
	/endmacro
/return

Sub Event_Channels(String line)
	/declare i int local
	/declare max int local ${line.Count[,]}
	/varcalc max ${max} + 1
	/declare chnl string local
	/for i 1 to ${max}
		/varset chnl ${line.Token[${i},,]}
		/if (${chnl.Find[Team]} > 0) {
			/varset channel ${chnl.Token[1,=]}
			/echo channel=${channel}
		}
	/next i
/return

Sub Event_Camp
	/echo Camping...
	/call endMacro
/return

Sub Event_Hail(String line, String Cible)
	/declare toTheZone string local
	/varset toTheZone ${Ini[${iniHail},General,${Cible.Lower}]}
	/if (${toTheZone.Length} && ${toTheZone.NotEqual[NULL]}) {
		/say ${toTheZone}
	} else {
		/echo ${line}
	}
	/return

Sub Event_Task(String line, String Tache)
	/chat ${channel} ${myMain} ${line}
/return

Sub Event_Assign(String line, String Tache)
	/chat ${channel} ${myMain} ${line}
/return

Sub Event_Tell(line, string from, string Msg)
	/declare toonName string local
	/call cleanToonName ${from}
	/varset toonName ${Macro.Return}
	/call isOneOfMyToons ${toonName}
	/if (${Macro.Return}) {
		/if (${Msg.Lower.Equal[loot]}) {
			/call LootMobs
		}
		/if (${Msg.Lower.Find[find].NotEqual[NULL]}) {
			
		}
	} else {
		/if (${Me.CleanName.NotEqual[${myMain}]}) {
			/chat ${channel} ${line}
		}
	}
/return

| "#1# tells Team:#2#, 'Follow #3#'"
Sub Event_Follow(String line, String from, String channelNumber, String who)
	/declare toonName string local
	/call cleanToonName ${from}
	/varset toonName ${Macro.Return}
	/call isOneOfMyToons ${toonName}
	/echo ${toonName}
	/echo ${NetBots[${toonName}].InZone} && ${Spawn[${toonName}].Distance} < 300 && ${Macro.Return}
	/if (${NetBots[${toonName}].InZone} && ${Spawn[${toonName}].Distance} < 300 && ${Macro.Return}) {
		/call amIconcerned ${toonName} ${who}
		/if (${Macro.Return}) {
			/call sticking ${toonName}
			/return
		}
	}
/return

Sub Event_Group(String line, String from)
	/declare toonName string local
	/call cleanToonName ${from}
	/varset toonName ${Macro.Return}
	/call isOneOfMyToons ${toonName}
	/if (${Macro.Return} && !${Me.Grouped}) {
		/invite
	}
/return

Sub Event_Zone(String line)
	/call joinChannel
	/echo ${Me.CleanName}'s corpse
	/tar ${Me.CleanName}'s corpse
	/if (${Target.ID} && ${Target.Distance} < 15) {
		/loot
	}
/return

Sub Event_EQBC(String EQBCSay, String EQBCSender, String EQBCCommand)
	/if (${EQBCCommand.Lower.Find[follow]}) {
		/call Event_Follow "" ${EQBCSender} "" ${EQBCCommand.Lower.Replace[follow ,]}
		/return
	}
	/if (${EQBCCommand.Lower.Find[gate]}) {
		/call gate ${EQBCSender} ${EQBCCommand.Lower.Replace[gate ,]}
		/return
	}
	/if (${EQBCCommand.Lower.Find[group]}) {
		/call makeGroup
	}
	/if (${EQBCCommand.Lower.Find[gh]}) {
		/call goToGuildHall ${EQBCSender} ${EQBCCommand.Lower.Replace[gh ,]}
	}
	/if (${EQBCCommand.Lower.Find[test]}) {
		/call test
	}
/return