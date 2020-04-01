open Base;
open Stdio;
open FeatureFlags;

type t = {featureFlags: array(featureFlags)};

let make = () => {featureFlags: [||]};

type action =
  | Replace(t)
  | ReplaceFeatureFlags(array(featureFlags))
  | EnableFeatureFlag(string)
  | DisableFeatureFlag(string);

let reducer = (action, state) =>
  switch (action) {
  | Replace(state) => state
  | ReplaceFeatureFlags(ffs) => {featureFlags: ffs}
  | EnableFeatureFlag(name) =>
    let featureFlags =
      Array.map(state.featureFlags, ~f=ff =>
        if (phys_equal(ff.name, name)) {
          {...ff, enabled: true};
        } else {
          ff;
        }
      );

    {featureFlags: featureFlags};
  | DisableFeatureFlag(name) =>
    let featureFlags =
      Array.map(state.featureFlags, ~f=ff =>
        if (phys_equal(ff.name, name)) {
          {...ff, enabled: false};
        } else {
          ff;
        }
      );

    {featureFlags: featureFlags};
  };
