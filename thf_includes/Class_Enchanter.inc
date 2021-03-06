|---------------------------------------------------------------------------------------------------------------------------------------------------|
|- Class_Enchanter.inc v1.0
|- Adapted for THF by Conradd (macro from Killians of PEQ)
|---------------------------------------------------------------------------------------------------------------------------------------------------|
|- Contains Enchanter specific functions related to,
|-		Setup and Ini entry information
|-		MainLoop, and AssistLoop hooks
|-		Over-all functionality / flow
|---------------------------------------------------------------------------------------------------------------------------------------------------|

#EVENT add_RuneTarget "<#1#> Rune on"
#EVENT add_RuneTarget "<#*#> Rune on #1#"
SUB EVENT_add_RuneTarget(string line, string RuneTarget)
	/if (${Me.Class.ShortName.Equal[ENC]}) {
		/if (${outerDebug}) /echo |- EVENT_add_RuneTarget ==>
	
		/if (!${SpawnCount[pc ${RuneTarget}]}) {
			/chat ${channelNumber} I don't see any PCs matching [${RuneTarget}]...
		} else {
		
			/varset RuneTarget ${Spawn[pc ${RuneTarget}].CleanName}
		
			/if (!${Defined[RuneTargets]}) {
				/declare RuneTargets[1] string outer
				/varset RuneTargets[1] ${RuneTarget}
				/chat ${channelNumber} Maintaining [${chainRune.Arg[1,/]}] on [${RuneTarget}].
			} else {
				
				/declare i int local
				
				/declare RuneTarget_Names string local
				
				/for i 1 to ${RuneTargets.Size}
				
					/varset RuneTarget_Names ${RuneTarget_Names}${RuneTargets[${i}]},
				
				/next i
				
				/if (${Select[${RuneTarget},${RuneTarget_Names}]}) {
					/chat ${channelNumber} I am already maintaining [${chainRune.Arg[1,/]}] on [${RuneTarget}].
				} else {
					/call BuildArray RuneTargets ${RuneTarget}
					/chat ${channelNumber} Maintaining [${chainRune.Arg[1,/]}] on [${RuneTarget_Names}${RuneTarget}].
				}
			}
		}
		
		/if (${outerDebug}) /echo <== add_RuneTarget -|
	}
/RETURN

#EVENT remove_RuneTarget "<#1#> Rune Off"
#EVENT remove_RuneTarget "<#*#> Rune Off #1#"
#EVENT remove_RuneTarget "<#*#> End Rune on #1#"
#EVENT remove_RuneTarget "<#*#> All runes off"
#EVENT remove_RuneTarget "<#*#> end runes"
SUB EVENT_remove_RuneTarget(line, ChatSender)
	/if (${Me.Class.ShortName.Equal[ENC]}) {
		/if (${outerDebug}) /echo |- remove_RuneTarget ==>

		/if (${Defined[RuneTargets]}) {

			/if (${Defined[ChatSender]}) {
			
				/declare i int local
				
				/for i 1 to ${RuneTargets.Size}

					/if (${RuneTargets[${i}].Equal[${ChatSender}]}) {
						/chat ${channelNumber} No longer maintaining [${chainRune.Arg[1,/]}] on [${RuneTargets[${i}]}].
						/call RemoveArrayElement RuneTargets "${RuneTargets[${i}]}"
					}
				
				/next i
				
			} else {
				/chat ${channelNumber} Ending chain runes.
				/deletevar RuneTargets
			}
			
		}
		
		/if (${outerDebug}) /echo <== remove_RuneTarget -|
	}
/RETURN

SUB check_Rune
	/if (${outerDebug}) /echo |- chainRune ==>

	/if (!${globalIdle} && ${RuneTargets.Size}) {
		
		/declare i int local
		
		/declare runeSpell string local ${chainRune.Arg[1,/]}
		/declare runeGem string local ${chainRune.Arg[2,/].Right[1]}
		/if (${chainRune.Arg[3,/].Length} && ${chainRune.Arg[3,/].Find[reagent::]}) /declare runeReagent string local ${chainRune.Arg[3,/].Arg[2,::]}
		
		/if (${Defined[runeReagent]} && !${FindItemCount[${runeReagent}]}) {
			/chat ${channelNumber} I have run out of [${runeReagent}] and can no longer maintain [${runeSpell}].  Ending chain runes.
			/deletevar RuneTargets
		} else {
		
			/for i 1 to ${RuneTargets.Size}
				/if (${Spawn[pc ${RuneTargets[${i}]}].Distance} < ${Spell[${runeSpell}].MyRange} && ${Me.CurrentMana} > ${Spell[${runeSpell}].Mana} && !${NetBots[${RuneTargets[${i}]}].Buff.Find[${Spell[${runeSpell}].ID}]}) {
					/call thf_Casting ${NetBots[${RuneTargets[${i}]}].ID} "${runeSpell}" gem${runeGem} ${If[${Me.Gem[${runeSpell}]},3s,${Spell[${runeSpell}].RecastTime}s]}
				}
			/if (!${outerActionTaken}) /next i
		}
	}

	/if (${outerDebug}) /echo <== chainRune -|
/RETURN

SUB ENC_Setup
	/if (${outerDebug}) /echo |- ENC_Setup ==>

	/if (${Ini[${iniToon},Enchanter,Chain Rune].Length}) /call iniToVar "${iniToon},Enchanter,Chain Rune" chainRune string outer

	/if (${outerDebug}) /echo <== ENC_Setup -|
/RETURN

SUB ENC_BackgroundEvents

	/doevents add_RuneTarget
	/doevents remove_RuneTarget

/RETURN

SUB ENC_MacroSettings
	/if (${outerDebug}) /echo |- ENC_MacroSettings ==>

	/call writeToIni "${iniTHF},Enchanter,Functions" "check_Rune/check_CombatBuffs/check_Burns/check_Debuffs/check_DoTs/check_Rain/check_Nukes/check_Buffs/check_Pets" FALSE

	/if (${outerDebug}) /echo <== ENC_MacroSettings -|
/RETURN



SUB ENC_CharacterSettings
/if (${outerDebug}) /echo |- ENC_CharacterSettings ==>

	/call writeToIni "${iniToon},Enchanter,Chain Rune"

/if (${outerDebug}) /echo <== ENC_CharacterSettings -|
/RETURN



SUB ENC_Aliases
/if (${outerDebug}) /echo |- ENC_Aliases ==>

	/squelch /alias /runeon /bc Rune on
	/squelch /alias /runeoff /bc Rune off

/if (${outerDebug}) /echo <== ENC_Aliases -|
/RETURN


|---------------------------------------------------------------------------------------------------------------------------------------------------|
|**
#EVENT AutoMez "<#*#> Mez All"
SUB EVENT_AutoMez
/if (${outerDebug}) /echo |- AutoMez -|- ==>
/if (!${Defined[AutoMez_Spell]} && ${Ini[${iniToon},Enchanter,Auto-Mez (Spell/Gem/MaxLevel)].Length}) {
	/call iniToVar "${iniToon},Enchanter,Auto-Mez (Spell/Gem/MaxLevel)" UNDEFINED string local
	/declare AutoMez_Spell string outer ${Macro.Return.Arg[1,/]}
	/declare AutoMez_Gem int outer ${Macro.Return.Arg[2,/]}
	/declare MaxMezLevel int outer ${Macro.Return.Arg[3,/]}
}
/call iniToVar ${Macro_Ini},Settings,outerAoeRadius Rain_Radius int outer
/declare i int local
/for i 1 to ${SpawnCount[npc radius ${Rain_Radius}]}
/if (${Spawn[${NearestSpawn[${i},NPC radius ${Rain_Radius}]}].LineOfSight} && ${Spawn[${NearestSpawn[${i},NPC radius ${Rain_Radius}]}].Type.NotEqual[Untargetable]} && ${Spawn[${NearestSpawn[${i},NPC radius ${Rain_Radius}]}].Type.NotEqual[Pet]} && ${Spawn[${NearestSpawn[${i},NPC radius ${Rain_Radius}]}].Level} <= ${MaxMezLevel}) {
	/call trueTarget ${NearestSpawn[${i},NPC radius ${Rain_Radius}].ID}
	/if (${Spawn[${Me.TargetOfTarget}].Type.Equal[PC]}) {
		/if (${outerDebug}) /echo Spawn: [${NearestSpawn[${i},NPC radius ${Rain_Radius}].CleanName},${NearestSpawn[${i},NPC radius ${Rain_Radius}].ID}] || TargetOfTarget: [${Me.TargetOfTarget}]
		/call MezTarget
	}
	/squelch /target clear
}
/delay 1
/next i
/if (${outerDebug}) {
	/echo |- AutoMez -|- <==
}
/RETURN
SUB MezTarget
/declare GiveUpValue int local 3
/if (${${Target.ID}${Spell[${AutoMez_Spell}].ID}} == 0) {
	/if (!${Defined[${Target.ID}${Spell[${AutoMez_Spell}].ID}]}) /declare ${Target.ID}${Spell[${AutoMez_Spell}].ID} timer outer 0
	:recast
	/call thf_Casting ${Target.ID} "${AutoMez_Spell}" Gem${AutoMez_Gem} 5s
	/if (${Select[${castReturn},CAST_RESISTED]}) {
		/if (!${Defined[ResistRetry]}) /declare ResistRetry int local 0
		/chat ${channelNumber} **Warning** ${Target.CleanName} resisted ${AutoMez_Spell}.
		/if (${Defined[ResistRetry]}) /varcalc ResistRetry ${ResistRetry}+1
		/if (${ResistRetry} < ${GiveUpValue}) /goto :recast
	} else /if (${Select[${castReturn},CAST_IMMUNE]}) {
		/varset ${Target.ID}${Spell[${AutoMez_Spell}].ID} 20000000
		/chat ${channelNumber} **Warning** ${Target.CleanName} is IMMUNE to ${AutoMez_Spell}.
	} else /if (${Select[${castReturn},CAST_CANNOTSEE]} && ${outerSpamTimer}==0) {
		/if (!${Defined[Retry]}) /declare LOSRetry int local 0
		/chat ${channelNumber} **Warning** I cannot see ${Target.CleanName}!!.
		/if (${Defined[outerSpamTimer]}) /varset outerSpamTimer 15s
		/if (${Defined[LOSRetry]}) /varcalc LOSRetry ${LOSRetry}+1
		/if (${LOSRetry} < ${GiveUpValue}) /goto :recast
	} else /if (${Select[${castReturn},CAST_OUTOFMANA]} && ${outerSpamTimer}==0) {
		/chat ${channelNumber} **Warning** I do not have enough mana to cast ${MyDebuffs[${i}]}!!.
		/if (${Defined[outerSpamTimer]}) /varset outerSpamTimer 15s
	} else /if (${Select[${castReturn},CAST_OUTOFRANGE]} && ${outerSpamTimer}==0) {
		/chat ${channelNumber} **Warning** ${MyDebuffs[${i}]} is out of range for ${MyDebuffs[${i}]}!!.
		/if (${Defined[outerSpamTimer]}) /varset outerSpamTimer 15s
	} else /if (${Select[${castReturn},CAST_SUCCESS]})	{
		/chat ${channelNumber} Landed ${AutoMez_Spell} on ${Target.CleanName}!!.
		/if (${Defined[${Target.ID}${Spell[${AutoMez_Spell}].ID}]}) /varset ${Target.ID}${Spell[${AutoMez_Spell}].ID} ${Math.Calc[${Spell[${AutoMez_Spell}].Duration} * 6-6]}s
	}
}
/delay 5s !${Me.Casting.ID}
/RETURN
#EVENT AutoMez_Off "<#*#> End AutoMez"
#EVENT AutoMez_Off "<#*#> AutoMez off"
SUB EVENT_AutoMez_Off
/if (${do_AutoMez}) {
	/chat ${channelNumber} I will no longer mez targets.
	/varset do_AutoMez FALSE
}
/RETURN
#EVENT AutoMez_On "<#*#> Start AutoMez"
#EVENT AutoMez_On "<#*#> AutoMez On"
SUB EVENT_AutoMez_On
/if (!${do_AutoMez}) {
	/chat ${channelNumber} Attempting to mez adds.
	/varset do_AutoMez TRUE
}
/RETURN