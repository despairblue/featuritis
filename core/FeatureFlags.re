type featureFlags = {
  name: string,
  enabled: bool,
};

module Query = [%graphql
  {|
    query featureFlags($type_: FeatureFlagsType!, $state: FeatureFlagState!) {
      featureFlags (type: $type_, state: $state) @bsRecord {
        name
        enabled
      }
    }
  |}
];

module EnableFeatureFlag = [%graphql
  {|
mutation activateGlobalFeatureFlags(
  $name: [String!]!
  $type_: FeatureFlagsType!
  $state: FeatureFlagState!
) {
  activateGlobalFeatureFlags(input: { names: $name }) {
    ... on ActivateGlobalFeatureFlagsPayloadError {
      message
    }
    ... on ActivateGlobalFeatureFlagsPayloadSuccess {
      featureFlags(type: $type_, state: $state) @bsRecord {
        name
        enabled
      }
    }
  }
}
  |}
];

module DisableFeatureFlag = [%graphql
  {|
mutation deactivateGlobalFeatureFlags(
  $name: [String!]!
  $type_: FeatureFlagsType!
  $state: FeatureFlagState!
) {
  deactivateGlobalFeatureFlags(input: { names: $name }) {
    ... on DeactivateGlobalFeatureFlagsPayloadError {
      message
    }
    ... on DeactivateGlobalFeatureFlagsPayloadSuccess {
      featureFlags(type: $type_, state: $state) @bsRecord {
        name
        enabled
      }
    }
  }
}
  |}
];
