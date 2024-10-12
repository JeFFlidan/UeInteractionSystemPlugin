## Table of contents
- [1. Interactor Component](#interactor-component)
- [2. Interactable Component](#interactable-component)

<a name="interactor-component"></a>
### 1. Interactor Component
To make player character interact with Actors with `Interactable Component`, you must add `Interactor Component` to character. `Interactor Component` does not require any configuration, it will work immediately after adding to the actor.

<a name="interactable-component"></a>
### 2. Interactable Component
To make `Actor` visible for character with `Interactor Component`, you must add `Interactable Component` to this `Actor`. This plugin gives two classes that you can use as parent for your interactable actors:
* `Interactable_SphereTrigger` uses `Sphere Component` as an area that will fire some events upon entering.
* `Interactable_BoxTrigger` uses `Box Component` as an area that will fire some events upon entering.

`Interactable Component` can be configured if you need particular functionality. All settings you can see in the picture below.

<p align="center">
    <img src="/Images/InteractableComponentSettings.png"/>
</p>

To make your `Interactable Component` fire `On Interact` event, you must have some interaction input logic in your game using `Enhanced Input Plugin`. To the selected `Input Action` you must bind `Interactor Component`'s method `Interact` (as mentioned earlier, character must have this component). To make actor with `Interactable Component` react to particular `Input Action`, you must use desired action as value for `Interaction Action` field.

To make `Interactable Component` fire `On Interact` event after holding button some time, you must set up `Interaction Input Type` to `Hold` and configure `Interaction Duration` to a non-zero time.

`Interactable Component` has three delegates for now:
* `On Begin Overlap` is broadcasting (activating) after character with `Interactor Component` enters sphere or box area;
* `On End OVerlap` is broadcasting after character with `Interactor Component` leaves sphere or box area;
* `On Interact` is broadcasting after calling `Interact` method of `Interactor Component`. In almost all cases, this method must be bound to an `Input Action`.

An example of binding a custom event to one of the delegate is below:

<p align="center">
    <img src="/Images/BindEvent.png"/>
</p>