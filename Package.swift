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
            targets: ["GMIPackage"])
        
    ],
    dependencies: [
        .package(name: "Realm", url: "https://github.com/realm/realm-cocoa", .exact(Version("10.5.0")))
    ],
    targets: [
        .target(name: "GMIPackage",
                        dependencies: [
                            .target(name: "GMI", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "IWS_Shared_Components", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "PalmAPI", condition: .when(platforms: .some([.iOS]))),
                            .product(name: "RealmSwift", package: "Realm"),
                            .product(name: "Realm", package: "Realm")
                        ],
                        path: "GMIPath"
                ),
        .binaryTarget(name: "GMI", url: "https://github.com/ImageWare/IWSAuthSDKiOS/raw/main/GMI.xcframework.zip", checksum: "e98052696a6285d8567e75a736f94727438ffe620a5a2572b0d5d4e847cc3c2c"),
        .binaryTarget(name: "IWS_Shared_Components",
                      path: "IWS_Shared_Components.xcframework"),
        
        .binaryTarget(name: "PalmAPI", path: "PalmAPI.xcframework")
    ],
    swiftLanguageVersions: [.v5]
)
