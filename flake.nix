{
  inputs.nixpkgs.url = "git+https://github.com/NixOS/nixpkgs?ref=nixpkgs-unstable";
  inputs.sprinkles.url = "git+https://codeberg.org/poacher/sprinkles";

  outputs = {
    self,
    nixpkgs,
    sprinkles,
    ...
  }: let
    forAllSystems = nixpkgs.lib.genAttrs ["x86_64-linux" "aarch64-linux"];
  in {
    packages = forAllSystems (system:
      (import ./. {inherit sprinkles;}).packages);
    devShells = forAllSystems (system:
      (import ./. {inherit sprinkles;}).shells);
  };
}
