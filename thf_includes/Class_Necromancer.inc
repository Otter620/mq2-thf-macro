|---------------------------------------------------------------------------------------------------------------------------------------------------|
|- Class_Necromancer.inc v1.0
|- Adapted for THF by Conradd (macro from Killians of PEQ)
|---------------------------------------------------------------------------------------------------------------------------------------------------|
|- Contains Necromancer specific functions related to,
|-		Setup and Ini entry information
|-		MainLoop, and AssistLoop hooks
|-		Over-all functionality / flow
|---------------------------------------------------------------------------------------------------------------------------------------------------|

SUB check_lifeTap
	/if (${outerDebug}) /echo |- check_LifeTap ==>
	
	/if (${outerAssisting}) {
		/declare lifeTap_Spell string local
		/declare lifeTap_Pct int local
		/declare i int local
		
		/for i 1 to ${lifeTaps.Size}
		
			/varset lifeTap_Spell ${lifeTaps[${i}].Arg[1,/]}
			
			|- /HealPct check
			/if (${lifeTaps[${i}].Find[/HealPct|]}) {
				/call argueString HealPct "${lifeTaps[${i}]}"	
				/varset lifeTap_Pct ${Macro.Return}
			}
		
			/if (${Me.PctHPs} < ${lifeTap_Pct}) {
				/call thf_Casting ${outerAssistTarget} "${lifeTap_Spell}/${If[${Me.Gem[${lifeTap_Spell}]},0,${Math.Calc[${Spell[${lifeTap_Spell}].RecastTime} + 3]}]}s"
			}
		
		/if (!${outerActionTaken}) /next i
	}

	/if (${outerDebug}) /echo <== check_LifeTap -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB check_castLich
	/if (${outerDebug}) /echo |- check_castLich ==>

	/if (${Defined[lichSpell]} && !${Me.Buff[${lichSpell}].ID} && ${Me.PctMana} < 90) {

		|- If I don't have lich on.
		/if (${Spell[${lichSpell}].Stacks}) /call thf_Casting ${Me.ID} "${lichSpell}/GiveUpTimer|${If[${Me.Gem[${lichSpell.Arg[1,/]}]},0,7s]}"
	}

	/if (${outerDebug}) /echo <== check_castLich -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB check_removeLich
	/if (${outerDebug}) /echo |- check_removeLich ==>

	/if (${Me.Buff[${lichSpell}].ID}) {
		/if (${Me.PctHPs} < 20 || ${Me.PctMana} == 100 && !${outerCombatTimer}) /nomodkey /notify BuffWindow Buff${Math.Calc[${Me.Buff[${lichSpell}].ID}-1].Int} leftmouseup
	}

	/if (${outerDebug}) /echo <== check_removeLich -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB check_manaDump
	/if (${outerDebug}) /echo |- check_manaDump ==>

	/if (${manaDump} && !${medBreak}) {
		/declare placeholder bool local true
		/declare manaDump_Target string local NULL
		/declare i int local
		
		/for i 1 to ${NetBots.Counts}
			
			|- If the netbot is me, skip it
			/if (${NetBots.Client[${i}].NotEqual[${Me}]}) {
				|- Check that NetBots(i) is a perspective manaDump_Target
				|-    |- 	Check								'group' tag								|	|-       						Class									|	|								Class.ShortName									|		|												Caster Classes															|	|													Healer Classes														|	|												Priest Classes															|
				/if (${Select[group,${manaDump_Targets}]} && ${Group.Member[${NetBots.Client[${i}]}]} || ${Select[${NetBots[${NetBots.Client[${i}]}].Class},${manaDump_Targets}]} || ${Select[${NetBots[${NetBots.Client[${i}]}].Class.ShortName},${manaDump_Targets}]} || ${Select[casters,${manaDump_Targets}]} && ${Select[${NetBots[${NetBots.Client[${i}]}].Class.ShortName},${CasterClasses}]} || ${Select[healers,${manaDump_Targets}]} && ${Select[${NetBots[${NetBots.Client[${i}]}].Class.ShortName},${PriestClasses}]} || ${Select[priests,${manaDump_Targets}]} && ${Select[${NetBots[${NetBots.Client[${i}]}].Class.ShortName},${PriestClasses}]}) {
					
					|- Check range to target
					/if (${SpawnCount[pc ${NetBots.Client[${i}]}]} && ${Spawn[pc ${NetBots.Client[${i}]}].Distance} < 200) {
						
						|- Find who has the lowest mana of all the potential targets
						/if (${NetBots[${NetBots.Client[${i}]}].PctMana} < ${NetBots[${manaDump_Target}].PctMana} || !${Bool[${manaDump_Target}]}) {
							/varset manaDump_Target ${NetBots.Client[${i}]}
							/if (${outerDebug}) /echo |- manaDump -|- manaDump_Target=${manaDump_Target}
						}
						
					}
				}
			}
		
		/next i

		/if (${Bool[${manaDump_Target}]}) {
			/if (${NetBots[${manaDump_Target}].PctMana} < ${manaDump_Pct} && ${Me.PctMana} > 5) {
			
				/if (${Me.SpellReady[${manaDump_Spells[1].Arg[1,/]}]} || !${Me.Gem[${manaDump_Spells[1].Arg[1,/]}]}) {
					/call thf_Casting ${NetBots[${manaDump_Target}].ID} "${manaDump_Spells[1]}/GiveUpTimer|${If[${Me.Gem[${manaDump_Spells[1].Arg[1,/]}]},0,5s]}"
				} else /if (${Me.SpellReady[${manaDump_Spells[2].Arg[1,/]}]} || !${Me.Gem[${manaDump_Spells[2].Arg[1,/]}]}) {
					/call thf_Casting ${NetBots[${manaDump_Target}].ID} "${manaDump_Spells[2]}/GiveUpTimer|${If[${Me.Gem[${manaDump_Spells[2].Arg[1,/]}]},0,5s]}"
				} else /if (${Me.SpellReady[${manaDump_Spells[3].Arg[1,/]}]} || !${Me.Gem[${manaDump_Spells[3].Arg[1,/]}]}) {
					/call thf_Casting ${NetBots[${manaDump_Target}].ID} "${manaDump_Spells[3]}/GiveUpTimer|${If[${Me.Gem[${manaDump_Spells[3].Arg[1,/]}]},0,5s]}"
				}
			}
		}
	}

	/if (${outerDebug}) /echo <== check_manaDump -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
#EVENT toggle_manaDump "<#1#> Mana Dump #2#"
SUB EVENT_toggle_manaDump(line, from, eventParams)
	/if (${outerDebug}) /echo |- EVENT_toggle_manaDump ==>

	|- Verify the event
	/call amIConcerned ${from} ${Me.CleanName} TRUE
	/if (${Defined[from]}) /if (${Defined[from]}) /varset from ${Macro.Return.Arg[1,_]}
	|-    |-----------Valid User----------|-    |--------Included Bot-----------|-    |-----------In Zone-------------|
	/if (${Bool[${Macro.Return.Arg[2,_]}]} && ${Bool[${Macro.Return.Arg[3,_]}]} && ${Bool[${Macro.Return.Arg[4,_]}]}) {
		
		/if (${eventParams.Find[/]}) /varset eventParams ${eventParams.Arg[1,/ ]}
		
		|- Turn off Mana Dump
		/if (${eventParams.Equal[end]} || ${eventParams.Equal[off]} || ${eventParams.Equal[stop]}) {
			/if (${manaDump}) {
				/chat ${channelNumber} Ending mana dump.
				/varset manaDump FALSE
			}
			
		|- Turn on mana dump
		} else {
		
			|- Are mana dump spells defined?
			/if (!${Defined[manaDump_Spells]}) {
				/chat ${channelNumber} You must define mana dump spells in my Bot Ini before engaging mana dump.
			} else {

			|- *** Set mana dump variables *** |
			
				/declare EVENT_Targets string local
				/declare EVENT_Pct int local NULL
				/declare i int local 1
			
				:next_Arg
				/if (${eventParams.Arg[${i}, ].Length}) {
				
					|- If the argument is an integer, set EVENT_Pct
					/if (${Int[${eventParams.Arg[${i}, ]}]}) {
						/varset EVENT_Pct ${eventParams.Arg[${i}, ]}
						/if (${outerDebug}) /echo EVENT_Pct=${EVENT_Pct}
						
					|- If the argument is NOT an integer, add it to the list of EVENT_Targets
					} else /if (!${eventParams.Arg[${i}, ].Equal[start]} && !${eventParams.Arg[${i}, ].Equal[On]})  {
						/varset EVENT_Targets ${EVENT_Targets}${eventParams.Arg[${i}, ]},
						/if (${outerDebug}) /echo EVENT_Targets=${EVENT_Targets}
					}
					
					/varcalc i ${i} + 1
					/goto :next_Arg
				}
				
				|- Set manaDump_Targets
				/if (${Bool[${EVENT_Targets}]}) /varset manaDump_Targets ${EVENT_Targets}
				/if (!${Bool[${manaDump_Targets}]}) /varset manaDump_Targets All
				/if (${manaDump_Targets.Right[1].Equal[,]}) /varset manaDump_Targets ${manaDump_Targets.Left[${Math.Calc[${manaDump_Targets.Length} - 1]}]}
				
				|- Set manaDump_Pct
				/if (${Bool[${EVENT_Pct}]}) /varset manaDump_Pct ${EVENT_Pct}
				/if (!${Bool[${manaDump_Pct}]}) /varset manaDump_Pct 95
				
				|- Set manaDump
				/if (${Defined[manaDump]}) {
					/varset manaDump TRUE
				} else {
					/declare manaDump bool outer TRUE
				}
				
				/chat ${channelNumber} Mana dumping [${manaDump_Targets}] who fall below [${manaDump_Pct}m].
			}
		}
	}

	/if (${outerDebug}) /echo <== EVENT_toggle_manaDump -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB NEC_Setup
	/if (${outerDebug}) /echo |- NEC_Setup ==>
	
	|- Lich
	/if (${Ini[${iniToon},${Me.Class},Lich Spell].Length}) /call iniToVar "${iniToon},${Me.Class},Lich Spell" lichSpell string outer
	
	|- LifeTap
	/if (${Ini[${iniToon},${Me.Class},LifeTaps].Length}) /call splitFromIni "${iniToon},${Me.Class},LifeTaps" lifeTaps outer /
	
	|- Mana Dump
	/if (${Ini[${iniToon},${Me.Class},Mana Dump (On/Off)].Length}) /call iniToVar "${iniToon},${Me.Class},Mana Dump (On/Off)" manaDump bool outer
	/declare manaDump_Pct int outer ${Ini[${iniToon},${Me.Class},Mana Dump Engage Pct]}
	/declare manaDump_Targets string outer ${Ini[${iniToon},${Me.Class},Who to Mana Dump]}
	/if (${Ini[${iniToon},${Me.Class},Mana Dumps].Length}) /call splitFromIni "${iniToon},${Me.Class},Mana Dumps" manaDump_Spells outer /
	
	|- Reset manaDump_Targets to replace spaces with commas
	/if (${manaDump_Targets.Find[ ]}) /varset manaDump_Targets ${manaDump_Targets.Replace[ ,]}

	/if (${outerDebug}) /echo <== NEC_Setup -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB NEC_BackgroundEvents

	/call check_removeLich

/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB NEC_MacroSettings
	/if (${outerDebug}) /echo |- NEC_MacroSettings ==>

	/call writeToIni "${iniTHF},Necromancer,Functions" "check_Burns/check_castLich/check_manaDump/check_Debuffs/check_DoTs/check_CombatBuffs/check_lifeTap/check_Nukes/check_petHeal/check_Buffs/check_Pets" FALSE

	/if (${outerDebug}) /echo <== NEC_MacroSettings -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB NEC_CharacterSettings
	/if (${outerDebug}) /echo |- NEC_CharacterSettings ==>

	|- Lich
	/call writeToIni "${iniToon},${Me.Class},Lich Spell"

	|- Life Taps
	/call writeToIni "${iniToon},${Me.Class},LifeTaps"

	|- Mana Dumps
	/call writeToIni "${iniToon},${Me.Class},Mana Dump (On/Off)"
	/call writeToIni "${iniToon},${Me.Class},Who to Mana Dump"
	/call writeToIni "${iniToon},${Me.Class},Mana Dump Engage Pct"
	/call writeToIni "${iniToon},${Me.Class},Mana Dumps"

	|- Twitch
	

	/if (${outerDebug}) /echo <== NEC_CharacterSettings -|
/RETURN

|---------------------------------------------------------------------------------------------------------------------------------------------------|
SUB NEC_Aliases
	/if (${outerDebug}) /echo |- NEC_Aliases ==>

	/noparse /squelch /alias /manadump /bc Mana dump

	/if (${outerDebug}) /echo <== NEC_Aliases -|
/RETURN


|---------------------------------------------------------------------------------------------------------------------------------------------------|
|**
	/if (${Ini[${iniToon},Necromancer,Allow Twitch requests (On/Off)].Length}) /call iniToVar "${iniToon},Necromancer,Allow Twitch requests (On/Off)" allowTwitch bool outer
	/if (${Ini[${iniToon},Necromancer,Min Mana to Allow Twitch Requests (Pct)].Length}) /call iniToVar "${iniToon},Necromancer,Min Mana to Allow Twitch Requests (Pct)" minTwitchMana int outer
	/call writeToIni "${iniToon},Necromancer,Allow Twitch requests (On/Off)"
	/call writeToIni "${iniToon},Necromancer,Min Mana to Allow Twitch Requests (Pct)"

#EVENT twitchMe "#*# tells you, 'Twitch on #1# #2#'"
#EVENT twitchMe "#*# tells the group, 'Twitch on #1# #2#'"
#EVENT twitchMe "#1# tells you, 'Twitch #2#'"
#EVENT twitchMe "#1# tells the group, 'Twitch #2#'"
SUB EVENT_twitchMe(line, ChatSender, int TwitchNum)
/if (${Me.Class.ShortName.Equal[NEC]} && ${TwitchNum}) {
/if (${outerDebug}) /echo |- EVENT_twitchMe ==>

	/if (${ChatSender.Left[1].Compare[ ]}<0) /varset ChatSender ${ChatSender.Right[-2].Left[-1]}

|---------------------------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------Check for PC in zone.
	/if (!${SpawnCount[pc ${ChatSender}]}) {
		/tell ${ChatSender} We're not in the same zone, dumbass!
	} else {
	
	|---------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------Check range.
		/if (${Spawn[pc ${ChatSender}].Distance} > 200) {
			/tell ${ChatSender} You're too far away to twitch
		} else {
	
		|---------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------If I'm not allowed to twitch
			/if (!${allowTwitch}) {
				/tell ${ChatSender} I'm not currently mana dumping others.  Try talking to my master, sucka!
			} else {
			
			|---------------------------------------------------------------------------------------------------------------------------------------------------|-----------------------------If I don't have enough mana to twitch
				/if (${Me.PctMana} < ${minTwitchMana} || ${Me.PctMana} <= 5) {
					/tell ${ChatSender} I do not have enough mana to twitch right now.  Try again later, sucka!
				} else {
				
				|---------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------If they ask for too much.
					/if (${TwitchNum} > 20 && !${NetBots[${ChatSender}].ID}) {
						/tell ${ChatSender} Whoa whoa whoa, hang on just a minute.  You want a mana battery, make your own necro.  You can requet twitches a maximum of [20] at a time.  Try again, sucka!
					} else {
					
					|---------------------------------------------------------------------------------------------------------------------------------------------------|---------------------Okay...I guess I'll twitch you.
						/if (${Verbosity}) /chat ${channelNumber} Twitching ${Spawn[${InFrom}].CleanName} ${TwitchNum} times.
						/tell ${ChatSender} Twitching you [${TwitchNum}] times.
						
						/declare twitchSpells[2] string local
							/varset twitchSpells[1] Covetous Subversion
							/varset twitchSpells[2] Sedulous Subversion
							
						/declare i int local
						
						/call trueTarget ${Spawn[pc ${InFrom}].ID}
						
					|---------------------------------------------------------------------------------------------------------------------------------------------------|----------------------Check for spells to be memmed
						/if (!${Me.Gem[Covetous Subversion]} || !${Me.Gem[Sedulous Subversion]}) {
							
							/declare spellsetMem_Timer timer local 100
							/chat ${channelNumber} Loading twitch spells...
							/memspellset TwitchSpells
							:Memloop
							/if (${outerDebug}) /echo |- EVENT_twitchMe -|- :Memloop
							
							/call check_BackgroundEvents
							/if (${outerDebug}) /echo spellsetMem_Timer = [${spellsetMem_Timer}]
							/if (${Window[SpellBookWnd].Open} && ${spellsetMem_Timer}) /goto :Memloop
							/if (${Me.Sitting}) /stand
						}
						
						/for i 1 to ${TwitchNum}
							
							/echo Twitching [${Spawn[pc ${ChatSender}].CleanName}] #[${i}] of [${TwitchNum}].
							:tryagain
							/if (${Me.SpellReady[Covetous Subversion]}) {
								/call thf_Casting ${Spawn[pc ${ChatSender}].ID} "Covetous Subversion" gem${Me.Gem[Covetous Subversion]} 3s
							} else /if (${Me.SpellReady[Sedulous Subversion]}) {
								/call thf_Casting ${Spawn[pc ${ChatSender}].ID} "Sedulous Subversion" gem${Me.Gem[Sedulous Subversion]} 3s
							} else /if (${Me.SpellReady[Rapacious Subvention]}) {
								/call thf_Casting ${Spawn[pc ${ChatSender}].ID} "Rapacious Subvention" gem${Me.Gem[Rapacious Subvention]} 3s
							} else {
								/goto :tryagain
							}
							
						/next i
					}
				}
			}
		}
	}
	
/if (${outerDebug}) /echo <== EVENT_twitchMe -|
}
/RETURN