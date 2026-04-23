# Jawia

Jawia is a personal Unreal Engine 5.7 RPG prototype built around systemic gameplay rather than a single isolated feature. The project combines exploration, party management, turn-based combat, dialogue tooling, itemization, save/load persistence, occult progression, and gesture-based ritual input.

Most of the reusable engineering work lives in custom C++ plugins, while Blueprints are used to wire those systems into game content, UI, encounters, and moment-to-moment player flow. The repository contains roughly 17k lines of custom C++ across the game module and the non-third-party plugins.

`Plugins/SPUD` is included in the repository as a third-party save system dependency and is not my code. The custom work in this project is concentrated in the game module and the plugins listed below.

## Project Focus

- Modular UE5 gameplay architecture built around custom plugins instead of one-off Blueprint logic.
- A limb-targeted, turn-based combat framework with abilities, effects, equipment, encounters, and persistent entity state.
- A reusable graph framework and custom dialogue editor/runtime for branching conversations.
- A gesture-recognition plugin for rune or ritual-style input using the $Q recognizer.
- Blueprint-facing systems designed to stay data-driven through assets, tags, and subsystem APIs.

## Custom Plugins

### ReTargetCombat

The main gameplay systems plugin and the largest part of the codebase.

- Turn-based combat manager with initiative ordering, combat phases, action resolution, and team-based win/loss handling.
- Limb-based combat model with per-body-part health, target resolution, destruction states, and body-part tags.
- Modular ability/effect pipeline built around Blueprintable abilities, effect classes, effect components, gameplay tags, and turn processing.
- Persistent entity framework for spawning, unloading, hydrating, dehydrating, and restoring characters from saved records.
- Encounter system with phased enemy waves, spawn anchors, combat deployment grids, Level Sequence hooks, retreat/disengage flow, and encounter persistence.
- Inventory and equipment systems with stackable items, catalogs, equip slots, runtime item instances, and item-granted effects or abilities.
- Party, social, occult, and save subsystems for party composition, NPC hidden relationship state, deity/ritual tracking, and SPUD-backed save orchestration.

### ReGraphBase

A reusable runtime/editor graph framework for branching systems.

- Runtime graph, node, edge, condition, variable, and graph-context model.
- Variable binding layer that can read from and write back to bound UObject properties.
- Custom asset editor with graph editing, selection details, copy/paste/duplicate support, and runtime graph synchronization.

### ReGraphDialogue

A dialogue system built on top of `ReGraphBase`.

- Custom dialogue graph asset type and editor integration.
- Runtime dialogue runner with lines, choices, conditional branches, events, sub-dialogues, conversation state checks, and execution tracing.
- Dialogue variables and gameplay-tag-driven conversation state.
- Graph compilation, validation, and auto-layout utilities for authoring support.

### ReSymbolRecognition

A runtime plugin for gesture/rune recognition.

- Implementation of the $Q Super-Quick Recognizer adapted for UE5.
- Point-cloud normalization, resampling, template loading, stroke-order variants, and async recognition.
- Input stroke collection component for mouse/touch-driven gesture capture.
- Template data assets for designer-authored gesture libraries.

## Game-Side Integration

Outside the plugins, the project contains Blueprint/gameplay content for:

- Playable characters and party members such as `Luse` and `Mark`
- Encounter and enemy content
- Main menu, inventory, party, and combat UI
- Interaction systems
- Ritual-related input mappings
- Weather and world systems

The root `Source/Jawia` module is intentionally lightweight compared to the plugin layer. The architecture is plugin-first, with project content assembling those systems into a playable prototype.

## Tech Stack

- Unreal Engine 5.7
- C++
- Blueprints
- Enhanced Input
- CommonUI
- Gameplay Tags
- Level Sequence
- SPUD for persistence integration

## Why This Repo Matters

This project is where I focused on systems engineering in Unreal rather than only level scripting or UI assembly. It shows how I structure gameplay features into reusable runtime/editor modules, expose them cleanly to designers, and connect persistence, combat, dialogue, inventory, and world-state systems into one project.

## Running The Project

1. Use Unreal Engine 5.7.
2. Open `Jawia.uproject`.
3. Let Unreal generate project files and build the C++ modules if prompted.
4. The default map is set in `Config/DefaultEngine.ini` and currently points to `/Game/NewMap.NewMap`.

## Repository Notes

- This is an active prototype, so some content names and assets are still work-in-progress.
- The strongest engineering work is in the custom plugins rather than the small root game module.
- For portfolio or CV review, the most important part of this repository is the custom systems layer: combat, persistence, dialogue tooling, itemization, and gesture recognition.
