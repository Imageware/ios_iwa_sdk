// swift-tools-version:5.3

import PackageDescription
import Foundation

let package = Package(
    name: "GMI",
    platforms: [
        .iOS(.v13)
    ],
    products: [
        .library(
            name: "GMIPackage",
            targets: ["GMIPackage"]),
    ],
    dependencies: [
        .package(name: "Realm", url: "https://github.com/realm/realm-cocoa", .exact(Version("10.5.0")))
    ],
    targets: [
        .binaryTarget(name: "GMI", url: "https://github.com/ImageWare/GMIPackage/raw/main/GMI.xcframework.zip", checksum: "57fb64a537e880b554532faa9ed9fb855ce84b2b7b8a6902efbb3edc95b4496d"),
        .binaryTarget(name: "IWS_Shared_Components", path: "IWS_Shared_Components.xcframework"),
        .binaryTarget(name: "PalmAPI", path: "PalmAPI.xcframework"),
        .target(name: "GMIPackage",
                        dependencies: [
                            .target(name: "GMI", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "IWS_Shared_Components", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "PalmAPI", condition: .when(platforms: .some([.iOS]))),
                            .product(name: "RealmSwift", package: "Realm")
                        ],
                        path: "GMIPath"
                )
    ]
)
